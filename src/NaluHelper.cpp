#include "Stream/NaluStream.h"
#include "Utils/NaluHelper.h"

namespace Utils {

void NaluHelper::ParseScalingList(Stream::NaluStream& ns, int* scalingList, int sizeOfScalingList, int* useDefaultScalingMatrixFlag) {
    int lastScale = 8;
    int nextScale = 8;

    for (int j = 0; j < sizeOfScalingList; ++j) {
        if (nextScale != 0) {
            int deltaScale = ns.readSev();
            nextScale = (lastScale + deltaScale + 256) % 256;
            useDefaultScalingMatrixFlag[0] = (j == 0 && nextScale == 0);
        }

        scalingList[j] = (nextScale == 0) ? lastScale : nextScale;
        lastScale = scalingList[j];
    }
}

void NaluHelper::ParseHrdParam(Stream::NaluStream& ns, Common::SPSVuiHrdParam_dt& hrdParam) {
    hrdParam.cpb_cnt_minus1 = ns.readUev();
    hrdParam.bit_rate_scale = ns.readNBits(4);
    hrdParam.cpb_size_scale = ns.readNBits(4);

    for(int SchedSelIdx = 0; SchedSelIdx <= hrdParam.cpb_cnt_minus1; ++SchedSelIdx) {
        hrdParam.bit_rate_value_minus1[SchedSelIdx] = ns.readUev();
        hrdParam.cpb_size_value_minus1[SchedSelIdx] = ns.readUev();
        hrdParam.cbr_flag[SchedSelIdx]              = ns.readOneBit();
    }

    hrdParam.initial_cpb_removal_delay_length_minus1 = ns.readNBits(5);
    hrdParam.cpb_removal_delay_length_minus1         = ns.readNBits(5);
    hrdParam.dpb_output_delay_length_minus1          = ns.readNBits(5);
    hrdParam.time_offset_length                      = ns.readNBits(5);
}

void NaluHelper::ParseVuiParam(Stream::NaluStream& ns, Common::SPSVuiParam_dt& vuiParam) {
    vuiParam.aspect_ratio_info_present_flag = ns.readOneBit();

    if(vuiParam.aspect_ratio_info_present_flag) {
        vuiParam.aspect_ratio_idc = ns.readNBits(8);

        if(Common::SAR_EXTEND == vuiParam.aspect_ratio_idc){
            vuiParam.sar_width  = ns.readNBits(16);
            vuiParam.sar_height = ns.readNBits(16);
        }
    }

    vuiParam.overscan_info_present_flag = ns.readOneBit();
    if(vuiParam.overscan_info_present_flag) {
        vuiParam.overscan_appropriate_flag = ns.readOneBit();
    }

    vuiParam.video_signal_type_present_flag = ns.readOneBit();
    if(vuiParam.video_signal_type_present_flag) {
        vuiParam.video_format                    = ns.readNBits(3);
        vuiParam.video_full_range_flag           = ns.readOneBit();
        vuiParam.colour_description_present_flag = ns.readOneBit();

        if(vuiParam.colour_description_present_flag) {
            vuiParam.colour_primaries         = ns.readNBits(8);
            vuiParam.transfer_characteristics = ns.readNBits(8);
            vuiParam.matrix_coefficients      = ns.readNBits(8);
        }
    }

    vuiParam.chroma_loc_info_present_flag = ns.readOneBit();
    if(vuiParam.chroma_loc_info_present_flag) {
        vuiParam.chroma_sample_loc_type_top_field    = ns.readUev();
        vuiParam.chroma_sample_loc_type_bottom_field = ns.readUev();
    }

    vuiParam.timing_info_present_flag = ns.readOneBit();
    if(vuiParam.timing_info_present_flag) {
        vuiParam.num_units_in_tick     = ns.readNBits(32);
        vuiParam.time_scale            = ns.readNBits(32);
        vuiParam.fixed_frame_rate_flag = ns.readOneBit();
    }

    vuiParam.nal_hrd_parameters_present_flag = ns.readOneBit();
    if(vuiParam.nal_hrd_parameters_present_flag) {
        NaluHelper::ParseHrdParam(ns, vuiParam.hrd_nal);
    }

    vuiParam.vcl_hrd_parameters_present_flag = ns.readOneBit();
    if(vuiParam.vcl_hrd_parameters_present_flag) {
        NaluHelper::ParseHrdParam(ns, vuiParam.hrd_vcl);
    }

    if(vuiParam.nal_hrd_parameters_present_flag || vuiParam.vcl_hrd_parameters_present_flag) {
        vuiParam.low_delay_hrd_flag = ns.readOneBit();
    }

    vuiParam.pic_struct_present_flag    = ns.readOneBit();
    vuiParam.bitstream_restriction_flag = ns.readOneBit();

    if(vuiParam.bitstream_restriction_flag) {
        vuiParam.motion_vectors_over_pic_boundaries_flag = ns.readOneBit();
        vuiParam.max_bytes_per_pic_denom                 = ns.readUev();
        vuiParam.max_bits_per_mb_denom                   = ns.readUev();
        vuiParam.log2_max_mv_length_horizontal           = ns.readUev();
        vuiParam.log2_max_mv_length_vertical             = ns.readUev();
        vuiParam.num_reorder_frames                      = ns.readUev();
        vuiParam.max_dec_frame_buffering                 = ns.readUev();
    }
}

bool NaluHelper::GetSliceType(uint32_t value, Common::Nalu_t type, uint32_t maxNumRefFrames, Common::Slice_t& ret) {
    value = value > 4 ? value % 5 : value;
    ret = static_cast<Common::Slice_t>(value);

    if (Common::Nalu_t::IDRSlice == type && Common::Slice_t::I_SLICE != ret && Common::Slice_t::SI_SLICE != ret) {
        // When nal_unit_type is equal to 5 (IDR picture), slice_type shall be equal to 2, 4, 7, or 9
        return false;
    }

    if (0 == maxNumRefFrames && Common::Slice_t::I_SLICE != ret && Common::Slice_t::SI_SLICE != ret) {
        // When max_num_ref_frames is equal to 0, slice_type shall be equal to 2, 4, 7, or 9
        return false;
    }

    return true;
}

void NaluHelper::ParseRefPicListModification(Stream::NaluStream& ns, Common::Slice_t slice, Common::SliceHeadParam_dt& sliceHeader) {
    auto& refPicListModParam = sliceHeader.ref_pic_list_modification;

    if (Common::Slice_t::I_SLICE != slice && Common::Slice_t::SI_SLICE!= slice) {
        refPicListModParam.ref_pic_list_modification_flag_l0 = ns.readOneBit();
        if (0 != refPicListModParam.ref_pic_list_modification_flag_l0) {
            do {
                refPicListModParam.modification_of_pic_nums_idc = ns.readUev();
                if (0 == refPicListModParam.modification_of_pic_nums_idc || 1 == refPicListModParam.modification_of_pic_nums_idc)  {
                    refPicListModParam.abs_diff_pic_num_minus1 = ns.readUev();
                }
                else if (2 == refPicListModParam.modification_of_pic_nums_idc) {
                    refPicListModParam.long_term_pic_num = ns.readUev();
                }

            } while(3 != refPicListModParam.modification_of_pic_nums_idc);
        }
    }

    if (Common::Slice_t::B_SLICE == slice) {
        refPicListModParam.ref_pic_list_modification_flag_l1 = ns.readOneBit();
        if (0 != refPicListModParam.ref_pic_list_modification_flag_l1) {
            do {
                refPicListModParam.modification_of_pic_nums_idc = ns.readUev();
                if (0 == refPicListModParam.modification_of_pic_nums_idc || 1 == refPicListModParam.modification_of_pic_nums_idc)  {
                    refPicListModParam.abs_diff_pic_num_minus1 = ns.readUev();
                }
                else if (2 == refPicListModParam.modification_of_pic_nums_idc) {
                    refPicListModParam.long_term_pic_num = ns.readUev();
                }

            } while(3!= refPicListModParam.modification_of_pic_nums_idc);
        }
    }
}

void NaluHelper::ParsePredWeightTable(Stream::NaluStream& ns, Common::Slice_t slice, uint32_t chromaArrayType,
    Common::SliceHeadParam_dt& sliceHeader) {
    auto& predWeightTableParam = sliceHeader.pred_weight_table;

    predWeightTableParam.luma_log2_weight_denom = ns.readUev();

    if (0 == chromaArrayType) {
        predWeightTableParam.chroma_log2_weight_denom = ns.readUev();
    }

    for (auto idx = 0; idx <= sliceHeader.num_ref_idx_l0_active_minus1; ++idx) {
        predWeightTableParam.luma_weight_l0_flag = ns.readOneBit();
        if (0 != predWeightTableParam.luma_weight_l0_flag) {
            predWeightTableParam.luma_weight_l0[idx] = ns.readSev();
            predWeightTableParam.luma_offset_l0[idx] = ns.readSev();
        }

        if (0 != chromaArrayType) {
            predWeightTableParam.chroma_weight_l0_flag = ns.readOneBit();
            if (0 != predWeightTableParam.chroma_weight_l0_flag) {
                for (auto jdx = 0; jdx < 2; ++jdx) {
                    predWeightTableParam.chroma_weight_l0[idx][jdx] = ns.readSev();
                    predWeightTableParam.chroma_offset_l0[idx][jdx] = ns.readSev();
                }
            }
        }
    }

    if (Common::Slice_t::B_SLICE == slice) {
        for (auto idx = 0; idx <= sliceHeader.num_ref_idx_l1_active_minus1; ++idx) {
            predWeightTableParam.luma_weight_l1_flag = ns.readOneBit();
            if (0 != predWeightTableParam.luma_weight_l1_flag) {
                predWeightTableParam.luma_weight_l1[idx] = ns.readSev();
                predWeightTableParam.luma_offset_l1[idx] = ns.readSev();
            }

            if (0 != chromaArrayType) {
                predWeightTableParam.chroma_weight_l1_flag = ns.readOneBit();
                if (0 != predWeightTableParam.chroma_weight_l1_flag) {
                    for (auto jdx = 0; jdx < 2; ++jdx) {
                        predWeightTableParam.chroma_weight_l1[idx][jdx] = ns.readSev();
                        predWeightTableParam.chroma_offset_l1[idx][jdx] = ns.readSev();
                    }
                }
            }
        }
    }
}

void NaluHelper::ParseDecRefPisMark(Stream::NaluStream& ns, Common::Nalu_t type, Common::SliceHeadParam_dt& sliceHeader) {
    auto& decRefPicMarkingParam = sliceHeader.dec_ref_pic_marking;

    if (Common::Nalu_t::IDRSlice == type) {
        decRefPicMarkingParam.no_output_of_prior_pics_flag = ns.readOneBit();
        decRefPicMarkingParam.long_term_reference_flag = ns.readOneBit();
    }
    else {
        decRefPicMarkingParam.adaptive_ref_pic_marking_mode_flag = ns.readOneBit();
        if (0 != decRefPicMarkingParam.adaptive_ref_pic_marking_mode_flag) {
            do {
                decRefPicMarkingParam.memory_management_control_operation = ns.readUev();
                if (1 == decRefPicMarkingParam.memory_management_control_operation ||
                    3 == decRefPicMarkingParam.memory_management_control_operation) {
                    decRefPicMarkingParam.difference_of_pic_nums_minus1 = ns.readUev();
                }

                if (2 == decRefPicMarkingParam.memory_management_control_operation) {
                    decRefPicMarkingParam.long_term_pic_num = ns.readUev();
                }

                if (3 == decRefPicMarkingParam.memory_management_control_operation ||
                    6 == decRefPicMarkingParam.memory_management_control_operation) {
                    decRefPicMarkingParam.long_term_frame_idx = ns.readUev();
                }

                if (4 == decRefPicMarkingParam.memory_management_control_operation) {
                    decRefPicMarkingParam.max_long_term_frame_idx_plus1 = ns.readUev();
                }

            } while(0 != decRefPicMarkingParam.memory_management_control_operation);
        }
    }
}

} // namespace Utils