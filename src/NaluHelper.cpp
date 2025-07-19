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

} // namespace Utils