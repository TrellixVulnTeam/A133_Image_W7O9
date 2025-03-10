/*
 * Copyright (C) 2019 Allwinnertech,  <liuli@allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include "include/eink_driver.h"
#include "include/eink_sys_source.h"
#include "include/fmt_convert.h"

static unsigned int continue_gu16_cnt;

bool is_upd_list_empty(struct buf_manager *buffer_mgr)
{
	bool ret = false;
	struct img_node *cur_img_node = NULL, *tmp_img_node = NULL;
	unsigned int count = 0;

	mutex_lock(&buffer_mgr->mlock);

	if (list_empty(&buffer_mgr->img_used_list)) {
		ret = true;
	} else {
		list_for_each_entry_safe(cur_img_node, tmp_img_node, &buffer_mgr->img_used_list, node) {
			if (cur_img_node->img->state == USED) {
				continue;/* last img may use calc upd_win or auto mode select */
			}
			count++;
		}
		if (count != 0)
			ret = false;
		else
			ret = true;
	}

	mutex_unlock(&buffer_mgr->mlock);

	return ret;
}

bool is_coll_list_empty(struct buf_manager *buffer_mgr)
{
	bool ret = false;
	struct img_node *cur_img_node = NULL, *tmp_img_node = NULL;
	unsigned int count = 0;

	mutex_lock(&buffer_mgr->mlock);

	if (list_empty(&buffer_mgr->img_collision_list)) {
		ret = true;
	} else {
		list_for_each_entry_safe(cur_img_node, tmp_img_node, &buffer_mgr->img_collision_list, node) {
			if (cur_img_node->img->state == USED) {
				EINK_DEBUG_MSG("img is used state!\n");
				continue;/* last img may use calc upd_win or auto mode select */
			}
			count++;
		}
		if (count != 0)
			ret = false;
		else
			ret = true;
	}

	mutex_unlock(&buffer_mgr->mlock);

	return ret;
}

int wait_for_newest_img_node(struct buf_manager *buf_mgr)
{
	struct img_node *cur_img_node = NULL, *tmp_img_node = NULL;
	struct img_node *newest_node = NULL, *free_node = NULL;
	u32 buf_size = buf_mgr->buf_size;

	mutex_lock(&buf_mgr->mlock);
	if (list_empty(&buf_mgr->img_used_list)) {
		goto collision_proc;
	}

	list_for_each_entry_safe(cur_img_node, tmp_img_node, &buf_mgr->img_used_list, node) {
		if (newest_node == NULL) {
			newest_node = cur_img_node;
		}
		if (cur_img_node->upd_order < newest_node->upd_order) {
			free_node = cur_img_node;
			if (free_node->img != NULL) {
				if (free_node->extra_flag == true) {
					free_node->extra_flag = false;
					__list_del_entry(&free_node->node);
					eink_free(free_node->img->vaddr, free_node->img->paddr, buf_size);
					kfree(free_node->img->eink_hist);
					kfree(free_node->img);
					kfree(free_node);
				} else if (free_node->img->de_bypass_flag == true) {
					free_node->img->de_bypass_flag = false;
					__list_del_entry(&free_node->node);
					kfree(free_node->img);
					kfree(free_node);
				} else {
					free_node->img->state = FREE;
					free_node->img->upd_mode = EINK_INIT_MODE;
					free_node->img->win_calc_en = true;
					free_node->img->win_calc_fin = false;
					free_node->img->mode_select_fin = false;
					free_node->update_master = -1;
					list_move_tail(&free_node->node, &buf_mgr->img_free_list);
				}
			}
		} else
			newest_node = cur_img_node;
	}

collision_proc:
	if (list_empty(&buf_mgr->img_collision_list)) {
		pr_info("[%s]:No img need to update!\n", __func__);
		mutex_unlock(&buf_mgr->mlock);
		return 0;
	}
	list_for_each_entry_safe(cur_img_node, tmp_img_node, &buf_mgr->img_collision_list, node) {
		if (newest_node == NULL) {
			newest_node = cur_img_node;
		}
		if (cur_img_node->upd_order < newest_node->upd_order) {
			free_node = cur_img_node;
			if (free_node->img != NULL) {
				if (free_node->extra_flag == true) {
					free_node->extra_flag = false;
					__list_del_entry(&free_node->node);
					eink_free(free_node->img->vaddr, free_node->img->paddr, buf_size);
					kfree(free_node->img->eink_hist);
					kfree(free_node->img);
					kfree(free_node);
				} else if (free_node->img->de_bypass_flag == true) {
					free_node->img->de_bypass_flag = false;
					__list_del_entry(&free_node->node);
					kfree(free_node->img);
					kfree(free_node);
				} else {
					free_node->img->state = FREE;
					free_node->img->upd_mode = EINK_INIT_MODE;
					free_node->img->win_calc_en = true;
					free_node->img->win_calc_fin = false;
					free_node->img->mode_select_fin = false;
					free_node->update_master = -1;
					list_move_tail(&free_node->node, &buf_mgr->img_free_list);
				}
			}
		} else
			newest_node = cur_img_node;
	}
	mutex_unlock(&buf_mgr->mlock);
	return 0;
}

bool rect_is_overlap(struct upd_win a_area, struct upd_win b_area)
{
	bool overlap_flag = true;

	if (is_upd_win_zero(a_area) || is_upd_win_zero(b_area)) {
		return false;
	}

	if ((max(a_area.left, b_area.left) <= min(a_area.right, b_area.right))
		&& (max(a_area.top, b_area.top) <= min(a_area.bottom, b_area.bottom))) {
		overlap_flag = true;
	} else {
		overlap_flag = false;
	}

	return overlap_flag;
}

/*
rectangle A include rectangle B, then return true;otherwise, return false
*/
static bool rect_include_judge(struct upd_win a_rect, struct upd_win b_rect)
{
	if ((a_rect.left <= b_rect.left) &&
		(a_rect.right >= b_rect.right) &&
		(a_rect.top <= b_rect.top) &&
		(a_rect.bottom >= b_rect.bottom)) {
		return true;
	} else {
		return false;
	}
}

#if 0
int rect_subtract_judge(struct upd_win org_area, struct upd_win sub_area, struct upd_win *output_area)
{
	bool partial_overlap = false;

	if (output_area == NULL) {
		pr_err("input param is NULL\n");
		return -1;
	}

	//is org_area and sub_area overlap?
	if (false == rect_is_overlap(org_area, sub_area)) {
		pr_err("two area is no overlap, cannot subtract\n");
		return -2;
	}

	//is sub_area include org_area?
	if (rect_include_judge(sub_area, org_area)) {
		pr_info("whole area is included,ORG:(%d,%d)~(%d,%d),SUB:(%d,%d)~(%d,%d)\n",
			org_area.left, org_area.top, org_area.right, org_area.bottom,
			sub_area.left, sub_area.top, sub_area.right, sub_area.bottom);
		return 0;
	}

	//is partial overlap
	if ((org_area.left == sub_area.left) && (org_area.right == sub_area.right)) {
		output_area->left = org_area.left;
		output_area->right = org_area.right;
		if (org_area.top == sub_area.top) {
			output_area->top = sub_area.bottom + 1;
			output_area->bottom = org_area.bottom;
			return 1;
		}

		if (org_area.bottom == sub_area.bottom) {
			output_area->top = org_area.top;
			output_area->bottom = sub_area.top - 1;
			return 1;
		}
	}


	if ((org_area.top == sub_area.top) && (org_area.bottom == sub_area.bottom)) {
		output_area->top = org_area.top;
		output_area->bottom = org_area.bottom;
		if (org_area.left == sub_area.left) {
			output_area->left = sub_area.right + 1;
			output_area->right = org_area.right;
			return 1;
		}

		if (org_area.right == sub_area.right) {
			output_area->left = org_area.left;
			output_area->right = sub_area.left - 1;
			return 1;
		}
	}

	pr_err("partial area cannot subtract\n");
	return -3;
}
#endif

bool rect_merge_check(struct upd_win new_rect, struct upd_win old_rect)
{
	bool ret = false;

	if (false == rect_is_overlap(new_rect, old_rect)) {
		return false;
	}

	if (rect_include_judge(new_rect, old_rect)) {
		ret = true;
	} else {
		if (((new_rect.left == old_rect.left) && (new_rect.right == old_rect.right)) ||
			((new_rect.top == old_rect.top) && (new_rect.bottom == old_rect.bottom))) {
			ret = true;
		} else {
			ret = false;
		}
	}

	return ret;
}

static struct img_node *try_to_merge_image(struct img_node *anode, struct img_node *bnode)
{
	struct img_node *tmp_anode = NULL, *tmp_bnode = NULL;

	if ((anode == NULL) || (bnode == NULL)) {
		pr_err("input node is null\n");
		return NULL;
	}

	if ((anode->img == NULL) || (bnode->img == NULL)) {
		pr_err("input image is null\n");
		return NULL;
	}

	tmp_anode = (anode->upd_order >= bnode->upd_order) ? anode : bnode;//new update
	tmp_bnode = (tmp_anode == anode) ? bnode : anode;//old update

	/* old update image is specify no merge, then return NULL*/
	if (tmp_bnode->img->force_fresh == true) {
		pr_warn("image specity no merge, mode=0x%x, order=%d\n",
			tmp_bnode->img->upd_mode, tmp_bnode->upd_order);
		return NULL;
	}

	if (false == rect_merge_check(tmp_anode->img->upd_win, tmp_bnode->img->upd_win)) {
		return NULL;
	}

	if ((tmp_bnode->img->upd_mode & 0xff) == EINK_DU_MODE) {
		pr_warn("du try merge\n");
	}

	tmp_anode->coll_flag |= tmp_bnode->coll_flag;

	tmp_anode->img->upd_win.left = min(tmp_anode->img->upd_win.left, tmp_bnode->img->upd_win.left);
	tmp_anode->img->upd_win.right = max(tmp_anode->img->upd_win.right, tmp_bnode->img->upd_win.right);
	tmp_anode->img->upd_win.top = min(tmp_anode->img->upd_win.top, tmp_bnode->img->upd_win.top);
	tmp_anode->img->upd_win.bottom = max(tmp_anode->img->upd_win.bottom, tmp_bnode->img->upd_win.bottom);

	return tmp_anode;
}

static struct img_node *get_img_from_coll_list(struct buf_manager *buffer_mgr)
{
	struct img_node *img_node = NULL, *combine_node = NULL, *free_node = NULL;
	struct img_node *cur_img_node = NULL, *tmp_img_node = NULL;
	unsigned int tmp_order = 0;

	u32 buf_size = buffer_mgr->buf_size;

	mutex_lock(&buffer_mgr->mlock);

	if (list_empty(&buffer_mgr->img_collision_list)) {
		mutex_unlock(&buffer_mgr->mlock);
		return NULL;
	}

	/* merge image node */
	list_for_each_entry_safe(cur_img_node, tmp_img_node, &buffer_mgr->img_collision_list, node) {
		if (cur_img_node->img->state == USED) {
			EINK_DEBUG_MSG("img is used dont merge!\n");
			continue;/* last img may use calc upd_win or auto mode select */
		}
		if (img_node == NULL) {
			img_node = cur_img_node;
		} else {
			//NO MERGE
			combine_node = try_to_merge_image(img_node, cur_img_node);
			if (combine_node == NULL) {
				EINK_DEBUG_MSG("merge block\n");
				break;
			} else {
				//merge success, free older image
				if (combine_node == img_node) {
					free_node = cur_img_node;
				} else {
					free_node = img_node;
					img_node = combine_node;
				}

				tmp_order = free_node->upd_order;
				if (free_node->img != NULL) {
					if (free_node->extra_flag == true) {
						free_node->extra_flag = false;
						__list_del_entry(&free_node->node);
						eink_free(free_node->img->vaddr, free_node->img->paddr, buf_size);
						kfree(free_node->img->eink_hist);
						kfree(free_node->img);
						kfree(free_node);
					} else if (free_node->img->de_bypass_flag == true) {
						free_node->img->de_bypass_flag = false;
						__list_del_entry(&free_node->node);
						kfree(free_node->img);
						kfree(free_node);
					} else {
						free_node->img->state = FREE;
						free_node->img->upd_mode = EINK_INIT_MODE;
						free_node->img->win_calc_en = true;
						free_node->img->win_calc_fin = false;
						free_node->img->mode_select_fin = false;
						free_node->update_master = -1;
						list_move_tail(&free_node->node, &buffer_mgr->img_free_list);
					}
				}
				EINK_INFO_MSG("merge successfully, free image(order=%d)\n", tmp_order);
			}
		}
	}

	img_node = NULL;
	list_for_each_entry_safe(cur_img_node, tmp_img_node, &buffer_mgr->img_collision_list, node) {
		if (cur_img_node->coll_flag == 0) {
			img_node = cur_img_node;
			break;
		}
	}

	if (img_node != NULL) {
		if ((img_node->img == NULL) || (img_node->img->state != CAN_USED)) {
			EINK_DEBUG_MSG("img_node NULL or busy\n");
			img_node = NULL;
		} else {
			EINK_INFO_MSG("get image node from collision list, order=%d\n", img_node->upd_order);
			img_node->img->state = USED;
		}
	} else {
		EINK_DEBUG_MSG("no valid collision node, please wait\n");
	}

	mutex_unlock(&buffer_mgr->mlock);
	return img_node;
}

struct img_node *get_img_from_upd_list(struct buf_manager *buffer_mgr)
{
	struct img_node *cur_img_node = NULL, *tmp_img_node = NULL;
	struct img_node *img_node = NULL, *combine_node = NULL, *free_node = NULL;
	unsigned int tmp_order = 0;

	u32 buf_size = buffer_mgr->buf_size;

	mutex_lock(&buffer_mgr->mlock);
	if (list_empty(&buffer_mgr->img_used_list)) {
		mutex_unlock(&buffer_mgr->mlock);
		return NULL;
	}

	list_for_each_entry_safe(cur_img_node, tmp_img_node, &buffer_mgr->img_used_list, node) {
		if (cur_img_node->img->state == USED) {
			EINK_DEBUG_MSG("img is used dont merge!\n");
			continue;/* last img may use calc upd_win or auto mode select */
		}

		if (img_node == NULL) {
			img_node = cur_img_node;
		} else {
			combine_node = try_to_merge_image(img_node, cur_img_node);
			if (combine_node == NULL) {
				EINK_DEBUG_MSG("merge block\n");
				break;
			} else {
				/* merge success, free older image */
				if (combine_node == img_node) {
					free_node = cur_img_node;
				} else {
					free_node = img_node;
					img_node = combine_node;
				}

				tmp_order = free_node->upd_order;
				EINK_INFO_MSG("%s: free node %p, state = %d\n", __func__, free_node, free_node->img->state);
				EINK_INFO_MSG("%s: img node %p, state = %d\n", __func__, img_node, img_node->img->state);
				if (free_node->img != NULL) {
					if (free_node->extra_flag == true) {
						free_node->extra_flag = false;
						__list_del_entry(&free_node->node);
						eink_free(free_node->img->vaddr, free_node->img->paddr, buf_size);
						kfree(free_node->img->eink_hist);
						kfree(free_node->img);
						kfree(free_node);
					} else if (free_node->img->de_bypass_flag == true) {
						free_node->img->de_bypass_flag = false;
						__list_del_entry(&free_node->node);
						kfree(free_node->img);
						kfree(free_node);
					} else {
						free_node->img->state = FREE;
						free_node->img->upd_mode = EINK_INIT_MODE;
						free_node->img->win_calc_en = true;
						free_node->img->win_calc_fin = false;
						free_node->img->mode_select_fin = false;
						free_node->update_master = -1;
						list_move_tail(&free_node->node, &buffer_mgr->img_free_list);
					}
				}
				EINK_INFO_MSG("merge successully, free image(order=%d)\n", tmp_order);
			}
		}
	}

	if (img_node != NULL) {
		if ((img_node->img == NULL) || (img_node->img->state != CAN_USED)) {
			EINK_DEBUG_MSG("img_node is NULL or Busy\n");
			img_node = NULL;
			goto out;
		} else {
			EINK_INFO_MSG("get img node sucess from upd list, order=%d\n", img_node->upd_order);
			img_node->img->state = USED;
		}
	} else {
		EINK_DEBUG_MSG("no valid image node for update\n");
	}

out:
	mutex_unlock(&buffer_mgr->mlock);

	return img_node;
}


bool check_upd_coll_state(struct buf_manager *mgr, u64 pipe_free_state)
{
	bool can_upd_flag = false;
	struct img_node *curnode = NULL, *tmpnode = NULL;

	mutex_lock(&mgr->mlock);
	list_for_each_entry_safe(curnode, tmpnode, &mgr->img_collision_list, node) {
		if (curnode)
			curnode->coll_flag &= (~pipe_free_state);
		if (curnode->coll_flag == 0) {
			can_upd_flag = true;
		}
	}
	mutex_unlock(&mgr->mlock);

	return can_upd_flag;
}

static int add_img_to_coll_list(struct buf_manager *buf_mgr, struct img_node *img_node)
{
	int ret = 0;
	struct img_node *curnode = NULL, *tmp_node = NULL;

	if ((buf_mgr == NULL) || (img_node == NULL)) {
		pr_err("%s:input param is null\n", __func__);
		return -1;
	}

	/* for buf list debug */
	if (eink_get_print_level() == 3) {
		EINK_INFO_MSG("Before Add Coll Image\n");
		print_coll_img_list(buf_mgr);
	}

	mutex_lock(&buf_mgr->mlock);
	list_for_each_entry_safe(curnode, tmp_node, &buf_mgr->img_collision_list, node) {
		if (curnode == img_node) {	//already add
			pr_debug("%s:image node is already here\n", __func__);
			goto out;
		}

	}

	__list_del_entry(&img_node->node);
	list_add_tail(&img_node->node, &buf_mgr->img_collision_list);
	/* if img is collison we cant set upd all en hardware limit */
	img_node->img->upd_all_en = false;

out:
	mutex_unlock(&buf_mgr->mlock);

	/* for debug */
	if (eink_get_print_level() == 3) {
		EINK_INFO_MSG("After Add Coll Image\n");
		print_coll_img_list(buf_mgr);
	}

	return ret;
}

static int remove_img_from_coll_list(struct buf_manager *buffer_mgr, struct img_node *img_node)
{
	int ret = -1;
	struct img_node *cur_img_node = NULL, *tmp_img_node = NULL;
	unsigned int tmp_order = 0;

	if ((buffer_mgr == NULL) || (img_node == NULL)) {
		pr_err("%s:input param is null\n", __func__);
		return -1;
	}

	/* for debug */
	if (eink_get_print_level() == 3) {
		EINK_INFO_MSG("Before Remove Coll Image\n");
		print_coll_img_list(buffer_mgr);
	}

	mutex_lock(&buffer_mgr->mlock);
	if (list_empty(&buffer_mgr->img_collision_list)) {
		pr_err("no collision image can be free\n");
		ret = 0;
		goto out;
	}

	list_for_each_entry_safe(cur_img_node, tmp_img_node, &buffer_mgr->img_collision_list, node) {
		if (img_node == cur_img_node) {
			tmp_order = cur_img_node->upd_order;
			if (cur_img_node->img != NULL) {
				cur_img_node->img->state = FREE;
				cur_img_node->img->upd_mode = EINK_INIT_MODE;
				cur_img_node->img->win_calc_en = true;
				cur_img_node->img->win_calc_fin = false;
				cur_img_node->img->mode_select_fin = false;
				cur_img_node->update_master = -1;
			}
			EINK_INFO_MSG("remove image node from collision list, order=%d\n", tmp_order);
			list_move_tail(&cur_img_node->node, &buffer_mgr->img_free_list);
			ret = 0;
			break;
		}
	}
out:
	mutex_unlock(&buffer_mgr->mlock);

	/* for debug */
	if (eink_get_print_level() == 3) {
		EINK_INFO_MSG("After Remove Coll Image\n");
		print_coll_img_list(buffer_mgr);
	}

	return ret;
}

static int convert_32bpp_to_gray(struct buf_manager *buf_mgr,
				struct disp_layer_config_inner *config,
				unsigned int layer_num,
				struct eink_img *last_img,
				struct eink_img *dest_img)
{
	int ret = 0;
#ifndef DE_WB_DEBUG
	struct fmt_convert_manager *cvt_mgr = NULL;

	cvt_mgr = get_fmt_convert_mgr(0);
	/* used DE hardware to convert 32bpp to 8bpp */
	if (config != NULL) {
		ret = cvt_mgr->start_convert(0, config, layer_num, last_img, dest_img);
	}
#else
	ret = eink_get_default_file_from_mem(dest_img->vaddr, DEFAULT_GRAY_PIC_PATH,
				(buf_mgr->width * buf_mgr->height), 0);

	dest_img->upd_win.left = 0;
	dest_img->upd_win.top = 0;
	dest_img->upd_win.right = buf_mgr->width - 1;
	dest_img->upd_win.bottom = buf_mgr->height - 1;
#endif

	if (ret < 0)
		pr_err("%s: fmt convert failed!\n", __func__);
	return ret;
}

int auto_mode_select(struct eink_img *last_img, struct eink_img *cur_img, u32 *auto_mode)
{
	int ret = 0;
	struct fmt_convert_manager *cvt_mgr = NULL;

	if (last_img == NULL || cur_img == NULL) {
		pr_err("%s:input para err or first!\n", __func__);
		return -EINVAL;
	}

	EINK_INFO_MSG("AUTO MODE SEL\n");
	cvt_mgr = get_fmt_convert_mgr(0);

	if ((last_img->de_bypass_flag == true) || (cur_img->de_bypass_flag == true))
		*auto_mode = EINK_DU_MODE;
	else {
		ret = cvt_mgr->fmt_auto_mode_select(cvt_mgr, last_img, cur_img);
		if (ret < 0)
			pr_err("%s:auto_mode calc failed!\n", __func__);
		*auto_mode = ret;
	}

	return ret;
}

static s32 set_global_clean_counter(struct buf_manager *buf_mgr, unsigned int cnt)
{
	if (cnt > 20) {
		pr_err("%s:global clean counter is to large\n", __func__);
		return -1;
	}

	mutex_lock(&buf_mgr->mlock);
	buf_mgr->global_clean_cnt = cnt;
	mutex_unlock(&buf_mgr->mlock);

	return 0;
}

bool check_valid_update_mode(enum upd_mode mode)
{
	bool ret = false;

	if (mode == EINK_AUTO_MODE) {
		return true;
	}

	switch (mode & 0xff) {
	case EINK_INIT_MODE:
	case EINK_DU_MODE:
	case EINK_GC16_MODE:
	case EINK_GC4_MODE:
	case EINK_A2_MODE:
	case EINK_GL16_MODE:
	case EINK_GLR16_MODE:
	case EINK_GLD16_MODE:
	case EINK_GU16_MODE:
		ret = true;
		break;
	default:
		ret = false;
		break;
	}

	return ret;
}

bool is_local_update_mode(enum upd_mode mode)
{
	bool ret = false;

	switch (mode & 0xff) {
	case EINK_DU_MODE:
		ret = true;
		break;
	default:
		ret = false;
		break;
	}
	return ret;
}


int buf_queue_image(struct buf_manager *buf_mgr,
		struct disp_layer_config_inner *config, unsigned int layer_num,
		struct eink_upd_cfg *upd_cfg)
{
	int ret = 0;
	struct img_node *curnode = NULL, *tmpnode = NULL, *de_bypass_node = NULL, *extra_node = NULL;
	struct eink_img *cur_img = NULL, *last_img = NULL;
	enum upd_mode update_mode = 0;
	u32 buf_size = 0;
	int i = 0;

	if ((buf_mgr == NULL) || (config == NULL)) {
		pr_err("%s:buf_mgr or config is null, please check\n", __func__);
		return -EINVAL;
	}
	/* for calc queue image use time */
	if (eink_get_print_level() == 8) {
		do_gettimeofday(&buf_mgr->stimer);
	}

	if (check_valid_update_mode(GET_UPDATE_MODE(upd_cfg->upd_mode)) == false) {
		pr_warn("%s:unknown mode(0x%08x), set AUTO MODE\n", __func__, upd_cfg->upd_mode);
		update_mode = GET_UPDATE_INFO(upd_cfg->upd_mode) | EINK_AUTO_MODE;
	} else
		update_mode = upd_cfg->upd_mode;


	EINK_INFO_MSG("caller info: order=%d out_fmt=0x%x upd_mode=0x%x, dither_mode=0x%x, debypass = %d, area(%d,%d)~(%d,%d)\n",
				buf_mgr->upd_order, upd_cfg->out_fmt, update_mode,
				upd_cfg->dither_mode, upd_cfg->de_bypass,
				upd_cfg->upd_win.left, upd_cfg->upd_win.top,
				upd_cfg->upd_win.right, upd_cfg->upd_win.bottom);
	EINK_INFO_MSG("caller info: upd_all_en=%d force_fresh=%d\n",
				upd_cfg->upd_all_en, upd_cfg->force_fresh);

	buf_size = buf_mgr->width * buf_mgr->height;

	/* for debug */
	if (eink_get_print_level() == 3) {
		EINK_INFO_MSG("Before Queue Image\n");
		print_used_img_list(buf_mgr);
		print_free_img_list(buf_mgr);
	}

	mutex_lock(&buf_mgr->mlock);

	/* user not use de to process */
	if (upd_cfg->de_bypass) {
		for (i = 0; i < 16; i++) {
			if (config[i].info.fb.fd && config[i].enable) {
				de_bypass_node = (struct img_node *)kmalloc(sizeof(struct img_node), GFP_KERNEL | __GFP_ZERO);
				if (de_bypass_node == NULL) {
					pr_err("%s:de_bypass node malloc failed!", __func__);
					mutex_unlock(&buf_mgr->mlock);
					return -ENOMEM;
				}

				de_bypass_node->item = eink_dma_map(config[i].info.fb.fd);/* -----不能只用config[0] */
				if (de_bypass_node->item == NULL) {
					pr_err("%s:[EINK]dma map item failed!", __func__);
					kfree(de_bypass_node);
					mutex_unlock(&buf_mgr->mlock);
					return -EINVAL;
				}

				de_bypass_node->buf_id = 15;
				de_bypass_node->upd_order = 0;
				de_bypass_node->coll_flag = 0;
				de_bypass_node->update_master = -1;
				de_bypass_node->img = (struct eink_img *)kmalloc(sizeof(struct eink_img), GFP_KERNEL | __GFP_ZERO);
				if (de_bypass_node->img == NULL) {
					pr_err("%s:de_bypass img malloc failed!", __func__);
					eink_dma_unmap(de_bypass_node->item);
					kfree(de_bypass_node);
					mutex_unlock(&buf_mgr->mlock);
					return -ENOMEM;
				}
				de_bypass_node->img->paddr = (void *)de_bypass_node->item->dma_addr;/* 这里可以增加kmap抓图层 */
				EINK_INFO_MSG("debypass dma_addr = %p\n", de_bypass_node->img->paddr);
				de_bypass_node->img->de_bypass_flag = true;
				de_bypass_node->img->state = FREE;
				de_bypass_node->img->size.width = config[i].info.fb.size[0].width;/* buf_mgr->width; */
				de_bypass_node->img->size.height = config[i].info.fb.size[0].height;/* buf_mgr->height; */
				de_bypass_node->img->size.align = 4;
				de_bypass_node->img->pitch = EINKALIGN(de_bypass_node->img->size.width,
						de_bypass_node->img->size.align);
				de_bypass_node->img->upd_mode = EINK_INIT_MODE;
				de_bypass_node->img->out_fmt = EINK_Y8;
				de_bypass_node->img->win_calc_fin = false;
				de_bypass_node->img->mode_select_fin = false;
				de_bypass_node->img->upd_all_en = false;

				curnode = de_bypass_node;
				goto add_img_list;
			}
		}
	}

	/* buffer slot has no free buffer use an extra buffer */
	if (list_empty(&buf_mgr->img_free_list)) {
		extra_node = (struct img_node *)kmalloc(sizeof(struct img_node), GFP_KERNEL | __GFP_ZERO);
		if (extra_node == NULL) {
			pr_err("%s:empty extra node malloc failed!", __func__);
			mutex_unlock(&buf_mgr->mlock);
			return -ENOMEM;
		}

		extra_node->img = (struct eink_img *)kmalloc(sizeof(struct eink_img), GFP_KERNEL | __GFP_ZERO);
		if (extra_node->img == NULL) {
			pr_err("%s:extra_node img malloc failed!", __func__);
			kfree(extra_node);
			mutex_unlock(&buf_mgr->mlock);
			return -ENOMEM;
		}

		extra_node->img->eink_hist =
			(unsigned int *)kmalloc((sizeof(unsigned int) * buf_mgr->gray_level_cnt), GFP_KERNEL | __GFP_ZERO);
		if (extra_node->img->eink_hist == NULL) {
			pr_err("%s:extra_node hist strcut malloc failed!\n", __func__);
			kfree(extra_node->img);
			kfree(extra_node);
			mutex_unlock(&buf_mgr->mlock);
			ret = -ENOMEM;
		}

		extra_node->img->vaddr = eink_malloc(buf_size, &extra_node->img->paddr);
		if (extra_node->img->vaddr == NULL) {
			pr_err("extra img buffer malloc failed!\n");
			kfree(extra_node->img->eink_hist);
			kfree(extra_node->img);
			kfree(extra_node);
			mutex_unlock(&buf_mgr->mlock);
			return -ENOMEM;
		}
		extra_node->extra_flag = true;
		extra_node->buf_id = 16;
		extra_node->upd_order = 0;
		extra_node->coll_flag = 0;
		extra_node->update_master = -1;
		extra_node->img->state = FREE;
		extra_node->img->size.width = buf_mgr->width;
		extra_node->img->size.height = buf_mgr->height;
		extra_node->img->size.align = 4;
		extra_node->img->pitch = EINKALIGN(extra_node->img->size.width,
						extra_node->img->size.align);
		extra_node->img->upd_mode = EINK_INIT_MODE;
		extra_node->img->out_fmt = EINK_Y8;
		extra_node->img->win_calc_fin = false;
		extra_node->img->mode_select_fin = false;
		extra_node->img->upd_all_en = false;

		curnode = extra_node;
		EINK_INFO_MSG("Extra buffer alloc success!\n");
	} else {
		EINK_INFO_MSG("GET CURNODE!\n");
		list_for_each_entry_safe(curnode, tmpnode, &buf_mgr->img_free_list, node) {
			if (curnode != NULL)
				break;
		}
		__list_del_entry(&curnode->node);
	}

add_img_list:
	curnode->upd_order = buf_mgr->upd_order++;

	if ((curnode->upd_order == 0) && (GET_UPDATE_MODE(upd_cfg->upd_mode) != EINK_INIT_MODE)) {
		update_mode = GET_UPDATE_INFO(upd_cfg->upd_mode) | EINK_GC16_MODE;
	}

	last_img = buf_mgr->last_img;
	cur_img = curnode->img;
	cur_img->upd_mode = update_mode;
	cur_img->force_fresh = upd_cfg->force_fresh;
	cur_img->upd_all_en = upd_cfg->upd_all_en;
	cur_img->dither_mode = upd_cfg->dither_mode;
	if (upd_cfg->out_fmt > 0)
		cur_img->out_fmt = upd_cfg->out_fmt;
	cur_img->state = CAN_USED; /* not used yet */

	EINK_INFO_MSG("eink config upd mode is 0x%x\n", update_mode);
	/* 回写得出的upd_win */
	/* first de win disable */
	if (curnode->upd_order == 0 || (upd_cfg->upd_mode == EINK_INIT_MODE)) {
		cur_img->upd_win.left = 0;
		cur_img->upd_win.top = 0;
		cur_img->upd_win.right = buf_mgr->width - 1;
		cur_img->upd_win.bottom = buf_mgr->height - 1;
		cur_img->win_calc_en = false;
		cur_img->upd_all_en = true;
	} else {
		if ((IS_RECT_UPDATE(update_mode) || (cur_img->de_bypass_flag == true))
				&& (is_upd_win_zero(upd_cfg->upd_win) == false)) {
			EINK_INFO_MSG("RECT MODE or Debypass %d use self upd_win\n",
							cur_img->de_bypass_flag);
			memcpy(&cur_img->upd_win, &upd_cfg->upd_win, sizeof(struct upd_win));
			cur_img->win_calc_en = false;
		} else {
			EINK_INFO_MSG("Use DE to calc upd win\n");
			cur_img->win_calc_en = true;
		}
	}

	if (upd_cfg->upd_mode == EINK_INIT_MODE) {
		EINK_INFO_MSG("EINK_INIT_MODE!\n");
		memset(cur_img->vaddr, 0xff, buf_size);
		ret = 0;
	} else if (cur_img->de_bypass_flag == false) {
		ret = convert_32bpp_to_gray(buf_mgr, config, layer_num, last_img, cur_img);
		if (ret || ((is_upd_win_zero(cur_img->upd_win) == true) && (upd_cfg->force_fresh == false))) {
			pr_err("%s:de wb convert failed ret = %d, or the same img abandon\n", __func__, ret);
			cur_img->state = FREE;
			if (curnode->extra_flag == true) {
				curnode->extra_flag = false;
				__list_del_entry(&curnode->node);
				eink_free(curnode->img->vaddr, curnode->img->paddr, buf_size);
				kfree(curnode->img->eink_hist);
				kfree(curnode->img);
				kfree(curnode);
				//buf放回free还是释放
			} else {
				EINK_INFO_MSG("Abandon unused node\n");
				list_add_tail(&curnode->node, &buf_mgr->img_free_list);
			}
			/* ret = -1; *//* the same img ret  = 0,err ret < 0*/
			goto out;
		}

		/* for debug capture wb data */
		if (eink_get_print_level() == 7) {
			eink_put_gray_to_mem(curnode->upd_order, (char *)cur_img->vaddr,
					buf_mgr->width, buf_mgr->height);
		}
	}

	if (last_img)
		last_img->win_calc_fin = true;

	if ((is_upd_win_zero(cur_img->upd_win) == true) && (upd_cfg->force_fresh == true)) {//有必要更新区域为0吗？
		EINK_INFO_MSG("maybe same pic but must be freshed\n");
		cur_img->upd_win.left = 0;
		cur_img->upd_win.top = 0;
		cur_img->upd_win.right = buf_mgr->width - 1;
		cur_img->upd_win.bottom = buf_mgr->height - 1;
	}

	if (cur_img->upd_win.left >= buf_mgr->width)
		cur_img->upd_win.left = buf_mgr->width - 1;

	if (cur_img->upd_win.top >= buf_mgr->height)
		cur_img->upd_win.top = buf_mgr->height - 1;

	if (cur_img->upd_win.right >= buf_mgr->width)
		cur_img->upd_win.right = buf_mgr->width - 1;

	if (cur_img->upd_win.bottom >= buf_mgr->height)
		cur_img->upd_win.bottom = buf_mgr->height - 1;

	if (IS_AUTO_MODE(update_mode)) {
		EINK_INFO_MSG("AUTO MODE IN!\n");
		ret = auto_mode_select(last_img, cur_img, &update_mode);
		if (ret < 0)
			update_mode = EINK_GU16_MODE;/* auto mode select fail use default*/
	}

	EINK_INFO_MSG("After auto mode sel upd mode = 0x%x\n", update_mode);
	cur_img->upd_mode = update_mode;

	if (last_img)
		last_img->mode_select_fin = true; //释放的位置？


	if (buf_mgr->global_clean_cnt != 0) {
		if (cur_img->upd_mode == EINK_GU16_MODE) {
			continue_gu16_cnt++;
			if (continue_gu16_cnt >= buf_mgr->global_clean_cnt) {
				continue_gu16_cnt = 0;
				cur_img->upd_all_en = true;
				cur_img->upd_mode = EINK_GC16_MODE;
			}
		} else {
			continue_gu16_cnt = 0;
		}
	} else {
		continue_gu16_cnt = 0;
	}

	buf_mgr->last_img = cur_img;
	ret = 0;

	EINK_INFO_MSG("add img to used list!\n");
	list_add_tail(&curnode->node, &buf_mgr->img_used_list);
out:
	if ((cur_img != NULL) && (curnode != NULL)) {
		if (cur_img->state == CAN_USED) {
			EINK_INFO_MSG("add to update list: order=%d, mode=0x%x, (%d,%d)~(%d,%d)\n",
					curnode->upd_order, cur_img->upd_mode,
					cur_img->upd_win.left, cur_img->upd_win.top,
					cur_img->upd_win.right, cur_img->upd_win.bottom);
		}
	}

	/* debug */
	if (eink_get_print_level() == 8) {
		do_gettimeofday(&buf_mgr->etimer);

		pr_info("order=%d, queue image take %d ms\n", curnode->upd_order,
				get_delt_ms_timer(buf_mgr->stimer, buf_mgr->etimer));
	}

	mutex_unlock(&buf_mgr->mlock);

	/* for debug */
	if (eink_get_print_level() == 3) {
		EINK_INFO_MSG("After Queue Image\n");
		print_used_img_list(buf_mgr);
		print_free_img_list(buf_mgr);
	}

	return ret;
}

s32 buf_dequeue_image(struct buf_manager *buf_mgr, struct img_node *img_node)
{
	int ret = 0;
	struct img_node *image_node = NULL, *tmp_node = NULL;
	unsigned int order = img_node->upd_order;

	if (img_node == NULL) {
		pr_err("image buffer node is null\n");
		return 0;
	}

	mutex_lock(&buf_mgr->mlock);
	list_for_each_entry_safe(image_node, tmp_node, &buf_mgr->img_used_list, node) {
		if (image_node != img_node) {
			continue;
		}

		order = image_node->upd_order;
		if (image_node->img != NULL) {
			image_node->img->state = FREE;
			image_node->img->upd_mode = EINK_INIT_MODE;
			image_node->img->win_calc_en = true;
			image_node->img->win_calc_fin = false;
			image_node->img->mode_select_fin = false;
			image_node->update_master = -1;
		}
		list_move_tail(&image_node->node, &buf_mgr->img_free_list);
		EINK_INFO_MSG("dequeue image buffer %d, order %d\n", img_node->buf_id, order);
	}

	mutex_unlock(&buf_mgr->mlock);

	/* EINK_INFO_MSG("dequeue image buffer %d, order %d\n", img_node->buf_id, order); */

	return ret;
}

int buf_mgr_init(struct eink_manager *eink_mgr)
{
	int ret = 0, i = 0;
	u32 gray_level_cnt = 0;
	struct buf_manager *buf_mgr = NULL;

	buf_mgr = (struct buf_manager *)kmalloc(sizeof(struct buf_manager), GFP_KERNEL | __GFP_ZERO);
	if (buf_mgr == NULL) {
		pr_err("buf manager malloc failed!\n");
		ret = -ENOMEM;
		goto mgr_err_out;
	}
	memset((void *)buf_mgr, 0, sizeof(struct buf_manager));

	gray_level_cnt = eink_mgr->panel_info.gray_level_cnt;

	buf_mgr->gray_level_cnt = gray_level_cnt;
	buf_mgr->width = eink_mgr->panel_info.width;
	buf_mgr->height = eink_mgr->panel_info.height;
	buf_mgr->buf_size = buf_mgr->width * buf_mgr->height;
	buf_mgr->upd_order = 0;
	buf_mgr->global_clean_cnt = DEFAULT_GC_COUNTER;
	buf_mgr->processing_img_node = NULL;

	mutex_init(&buf_mgr->mlock);
	INIT_LIST_HEAD(&buf_mgr->img_free_list);
	INIT_LIST_HEAD(&buf_mgr->img_used_list);
	INIT_LIST_HEAD(&buf_mgr->img_collision_list);

	for (i = 0; i < MAX_IMG_CNT; i++) {
		buf_mgr->img_node[i].buf_id = i;
		buf_mgr->img_node[i].upd_order = 0;
		buf_mgr->img_node[i].coll_flag = 0;
		buf_mgr->img_node[i].update_master = -1;
		buf_mgr->img_node[i].img = (struct eink_img *)kmalloc(sizeof(struct eink_img), GFP_KERNEL | __GFP_ZERO);
		if (buf_mgr->img_node[i].img == NULL) {
			pr_err("%s:img strcut malloc failed!\n", __func__);
			ret = -ENOMEM;
			goto img_node_err;
		}
		memset(buf_mgr->img_node[i].img, 0, sizeof(struct eink_img));

		buf_mgr->img_node[i].img->state = FREE;
		buf_mgr->img_node[i].img->size.width = buf_mgr->width;
		buf_mgr->img_node[i].img->size.height = buf_mgr->height;
		buf_mgr->img_node[i].img->size.align = 4;//看看哪里会用到
		buf_mgr->img_node[i].img->pitch =
					EINKALIGN(buf_mgr->img_node[i].img->size.width,
					buf_mgr->img_node[i].img->size.align);
		buf_mgr->img_node[i].img->upd_mode = EINK_INIT_MODE;
		buf_mgr->img_node[i].img->out_fmt = EINK_Y8;
		buf_mgr->img_node[i].img->win_calc_fin = false;
		buf_mgr->img_node[i].img->mode_select_fin = false;
		buf_mgr->img_node[i].img->upd_all_en = false;
		buf_mgr->img_node[i].img->eink_hist =
			(unsigned int *)kmalloc((sizeof(unsigned int) * gray_level_cnt), GFP_KERNEL | __GFP_ZERO);
		if (buf_mgr->img_node[i].img->eink_hist == NULL) {
			pr_err("%s:eink hist strcut malloc failed!\n", __func__);
			ret = -ENOMEM;
			goto hist_node_err;
		}

		buf_mgr->img_node[i].img->vaddr = eink_malloc(buf_mgr->buf_size, &buf_mgr->img_node[i].img->paddr);
		if (buf_mgr->img_node[i].img->vaddr == NULL) {
			pr_err("img buffer malloc failed!\n");
			ret = -ENOMEM;
			goto img_buffer_err;
		}
		EINK_INFO_MSG("image %d, paddr=0x%p, vaddr=0x%p\n",
				i, buf_mgr->img_node[i].img->paddr, buf_mgr->img_node[i].img->vaddr);

		/* init mode need buf data 0xff */
		memset((void *)buf_mgr->img_node[i].img->vaddr, 0xff, buf_mgr->buf_size);
		list_add_tail(&buf_mgr->img_node[i].node, &buf_mgr->img_free_list);
	}


	//	is_full//有点问题

	buf_mgr->coll_img_workqueue = alloc_workqueue("EINK_COLL_WORK",
			WQ_HIGHPRI | WQ_MEM_RECLAIM | WQ_UNBOUND, 1);
	INIT_WORK(&buf_mgr->coll_handle_work, upd_coll_win_irq_handler);

	buf_mgr->is_upd_list_empty = is_upd_list_empty;
	buf_mgr->is_coll_list_empty = is_coll_list_empty;
	buf_mgr->add_img_to_coll_list = add_img_to_coll_list;
	buf_mgr->remove_img_from_coll_list = remove_img_from_coll_list;
	buf_mgr->check_upd_coll_state = check_upd_coll_state;
	buf_mgr->get_img_from_upd_list = get_img_from_upd_list;
	buf_mgr->get_img_from_coll_list = get_img_from_coll_list;
	buf_mgr->queue_image = buf_queue_image;
	buf_mgr->dequeue_image = buf_dequeue_image;
	buf_mgr->set_global_clean_counter = set_global_clean_counter;
	buf_mgr->wait_for_newest_img_node = wait_for_newest_img_node;

	eink_mgr->buf_mgr = buf_mgr;

	return ret;

img_buffer_err:
	for (i = 0; i < MAX_IMG_CNT; i++) {
		if (buf_mgr->img_node[i].img->vaddr) {
			eink_free(buf_mgr->img_node[i].img->vaddr,
					(void *)buf_mgr->img_node[i].img->paddr,
					buf_mgr->buf_size);
			buf_mgr->img_node[i].img->vaddr = NULL;
		}
	}

hist_node_err:
	for (i = 0; i < MAX_IMG_CNT; i++) {
		kfree(buf_mgr->img_node[i].img->eink_hist);
	}

img_node_err:
	for (i = 0; i < MAX_IMG_CNT; i++) {
		kfree(buf_mgr->img_node[i].img);
	}
mgr_err_out:
	kfree(buf_mgr);
	return ret;

}
