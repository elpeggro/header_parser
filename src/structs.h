#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <cstdint>
#include <string>
#include <vector>

/**
 * MP4 box header is always 8 bytes:
 * size: 4 bytes
 * name: 4 bytes
 * To get to the data of an mdat box calculate location + 8
 * location + size points to the next MP4 box.
 */
typedef struct {
  const uint8_t *location;
  uint32_t location_relative;
  uint32_t size;
  std::string name;
} MP4Box;

/** NAL unit header is always 5 bytes:
 * size:               4 bytes
 * forbidden_zero_bit: 1 bit
 * nal_ref_idc:        2 bits
 * nal_unit_type:      5 bits
 * To get to the rbsp location calculate location + 5
 * The size written in the bytestream is EXCLUDING itself, but we include it to make the semantics the same as with
 * the MP4 headers. So location + size points to the end of the NAL unit, i.e., the beginning of the next NAL unit or
 * the next MP4 box.
 */
typedef struct {
  // IdrPicFlag = ( ( nal_unit_type = = 5 ) ? 1 : 0 )
  const uint8_t *location;
  uint32_t location_relative;
  uint32_t size;
  uint8_t nal_ref_idc;
  uint8_t nal_unit_type;
  uint32_t slice_header_size;
} NALUnit;

/**
 * Sequence Parameter Set struct.
 * Missing fields:
 *   seq_scaling_list_present_flag and scaling_list() (7.3.2.1.1.1 in the standard)
 *   Everything in the else if( pic_order_cnt_type == 1 ) branch: delta_pic_order_always_zero_flag, ...
 *   vui_parameters() (E.1.1)
 */
typedef struct {
  uint8_t profile_idc;
  bool constraint_set0_flag;
  bool constraint_set1_flag;
  bool constraint_set2_flag;
  bool constraint_set3_flag;
  bool constraint_set4_flag;
  bool constraint_set5_flag;
  uint8_t level_idc;
  uint32_t seq_parameter_set_id;
  uint32_t chroma_format_idc;
  bool separate_colour_plane_flag;
  uint32_t bit_depth_luma_minus8;
  uint32_t bit_depth_chroma_minus8;
  bool qpprime_y_zero_transform_bypass_flag;
  bool seq_scaling_matrix_present_flag;
  // TODO seq_scaling_list_present_flag, ...
  uint32_t log2_max_frame_num_minus4;
  uint32_t pic_order_cnt_type;
  uint32_t log2_max_pic_order_cnt_lsb_minus4;
  // TODO delta_pic_order_always_zero_flag, ...
  bool delta_pic_order_always_zero_flag;
  int32_t offset_for_non_ref_pic;
  int32_t offset_for_top_to_bottom_field;
  uint32_t num_ref_frames_in_pic_order_cnt_cycle;
  std::vector<int32_t> offset_for_ref_frame;
  uint32_t max_num_ref_frames;
  bool gaps_in_frame_num_value_allowed_flag;
  uint32_t pic_width_in_mbs_minus1;
  uint32_t pic_height_in_map_units_minus1;
  bool frame_mbs_only_flag;
  bool mb_adaptive_frame_field_flag;
  bool direct_8x8_inference_flag;
  bool frame_cropping_flag;
  uint32_t frame_crop_left_offset;
  uint32_t frame_crop_right_offset;
  uint32_t frame_crop_top_offset;
  uint32_t frame_crop_bottom_offset;
  bool vui_parameters_present_flag;
  // TODO vui_parameters()
} SPS;

/**
 * Picture Parameter Set struct.
 * Missing fields:
 *   Everything in the if( more_rbsp_data() ) { branch: transform_8x8_mode_flag, scaling_list(), ...
 */
typedef struct {
  uint32_t pic_parameter_set_id;
  uint32_t seq_parameter_set_id;
  bool entropy_coding_mode_flag;
  bool bottom_field_pic_order_in_frame_present_flag;
  uint32_t num_slice_groups_minus1;
  uint32_t slice_group_map_type;
  std::vector<uint32_t> run_length_minus1;
  std::vector<uint32_t> top_left;
  std::vector<uint32_t> bottom_right;
  bool slice_group_change_direction_flag;
  uint32_t slice_group_change_rate_minus1;
  uint32_t pic_size_in_map_units_minus1;
  std::vector<uint32_t> slice_group_id;
  uint32_t num_ref_idx_l0_default_active_minus1;
  uint32_t num_ref_idx_l1_default_active_minus1;
  bool weighted_pred_flag;
  uint8_t weighted_bipred_idc;
  int32_t pic_init_qp_minus26;
  int32_t pic_init_qs_minus26;
  int32_t chroma_qp_index_offset;
  bool deblocking_filter_control_present_flag;
  bool constrained_intra_pred_flag;
  bool redundant_pic_cnt_present_flag;
  // TODO if( more_rbsp_data( ) )
} PPS;

typedef struct {
  uint8_t *first_mb_in_slice;
  uint32_t slice_type;
  uint32_t pic_parameter_set_id;
  uint8_t colour_plane_id;
  uint32_t frame_num; // length: log2_max_frame_num_minus4 + 4
  bool field_pic_flag;
  bool bottom_field_flag;
  uint32_t idr_pic_id; // IdrPicFlag = ( ( nal_unit_type == 5 ) ? 1 : 0 )
  uint32_t pic_order_cnt_lsb; // length: log2_max_pic_order_cnt_lsb_minus4 + 4
  int32_t delta_pic_order_cnt_bottom;
  int32_t delta_pic_order_cnt_0;
  int32_t delta_pic_order_cnt_1;
  uint32_t redundant_pic_cnt;
  bool direct_spatial_mv_pred_flag;
  bool num_ref_idx_active_override_flag;
  uint32_t num_ref_idx_l0_active_minus1;
  uint32_t num_ref_idx_l1_active_minus1;
  // TODO ref_pic_list_mvc_modification()
  // ==== ref_pic_list_modification() ====
  bool ref_pic_list_modification_flag_l0;
  uint32_t modification_of_pic_nums_idc;
  uint32_t abs_diff_pic_num_minus1;
  uint32_t long_term_pic_num;
  // =====================================
  bool ref_pic_list_modification_flag_l1;
  // ===== pred_weight_table() ====
  uint32_t luma_log2_weight_denom;
  uint32_t chroma_log2_weight_denom;
  bool luma_weight_l0_flag;
  std::vector<int32_t> luma_weight_l0;
  std::vector<int32_t> luma_offset_l0;
  bool chroma_weight_l0_flag;
  std::vector<std::pair<int32_t, int32_t>> chroma_weight_l0;
  std::vector<std::pair<int32_t, int32_t>> chroma_offset_l0;
  bool luma_weight_l1_flag;
  std::vector<int32_t> luma_weight_l1;
  std::vector<int32_t> luma_offset_l1;
  bool chroma_weight_l1_flag;
  std::vector<std::pair<int32_t, int32_t>> chroma_weight_l1;
  std::vector<std::pair<int32_t, int32_t>> chroma_offset_l1;
  // ==============================
  // ==== dec_ref_pic_marking() ====
  bool no_output_of_prior_pics_flag;
  bool long_term_reference_flag;
  bool adaptive_ref_pic_marking_mode_flag;
  uint32_t memory_management_control_operation;
  uint32_t difference_of_pic_nums_minus1;
  // uint32_t long_term_pic_num; Double assignment?
  uint32_t long_term_frame_idx;
  uint32_t max_long_term_frame_idx_plus1;
  // ===============================
  uint32_t cabac_init_idc;
  int32_t slice_qp_delta;
  bool sp_for_switch_flag;
  int32_t slice_qs_delta;
  uint32_t disable_deblocking_filter_idc;
  int32_t slice_alpha_c0_offset_div2;
  int32_t slice_beta_offset_div2;
  uint32_t slice_group_change_cycle;
} SliceHeader;

#endif //STRUCTS_H_
