/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Product name: redemption, a FLOSS RDP proxy
   Copyright (C) Wallix 2010-2013
   Author(s): Christophe Grosjean, Javier Caverni, Raphael Zhou

   Error exception object
*/

#ifndef _REDEMPTION_CORE_ERROR_HPP_
#define _REDEMPTION_CORE_ERROR_HPP_

enum {
    NO_ERROR = 0,
    ERR_STREAM_MEMORY_TOO_SMALL,
    ERR_STREAM_MEMORY_ALLOCATION_ERROR,
    ERR_STREAM_VALUE_TOO_LARGE_FOR_OUT_BER_LEN_UINT7,
    ERR_BITMAP_CACHE_ITEM_MEMORY_ALLOCATION_FAILED,
    ERR_SUBSTREAM_OVERFLOW_IN_CONSTRUCTOR,
    ERR_SUBSTREAM_OVERFLOW_IN_RESIZE,
    ERR_STREAM_VALUE_TOO_LARGE_FOR_RESERVED_LEADING_SPACE,
    ERR_STREAM_RESERVED_LEADING_SPACE_TOO_SMALL,

    ERR_MEMORY_ALLOCATION_FAILED = 200,

    ERR_SOCKET_CONNECT_FAILED = 1000,
    ERR_SOCKET_GETHOSTBYNAME_FAILED,
    ERR_SOCKET_ALLREADY_CLOSED,
    ERR_SOCKET_CLOSED,
    ERR_SOCKET_ERROR,

    ERR_TRANSPORT = 1500,
    ERR_TRANSPORT_NO_MORE_DATA,
    ERR_TRANSPORT_WRITE_FAILED,
    ERR_TRANSPORT_CLOSED,
    ERR_TRANSPORT_READ_FAILED,
    ERR_TRANSPORT_INPUT_ONLY_USED_FOR_RECV,
    ERR_TRANSPORT_OUTPUT_ONLY_USED_FOR_SEND,
    ERR_TRANSPORT_DIFFERS,
    ERR_TRANSPORT_OPEN_FAILED,
    ERR_TRANSPORT_SEEK_NOT_AVAILABLE,
    ERR_TRANSPORT_SEEK_FAILED,

    ERR_TRANSPORT_TLS_CONNECT_FAILED = 1600,

    ERR_ACL_UNEXPECTED_IN_ITEM_OUT = 1700,
    ERR_ACL_MESSAGE_TOO_BIG,

    ERR_SESSION_TERMINATED = 2000,

    ERR_T123_EXPECTED_TPKT_VERSION_3,

    ERR_X224 = 3000,
    ERR_X224_SENDING_UNKNOWN_PDU_TYPE,
    ERR_X224_EXPECTED_DATA_PDU,
    ERR_X224_EXPECTED_CONNECTION_CONFIRM,

    ERR_ISO_INCOMING_CODE_NOT_PDU_CR = 4000,
    ERR_ISO_INCOMING_BAD_PDU_CR_LENGTH,
    ERR_ISO_RECV_MSG_VER_NOT_3,
    ERR_ISO_RECV_CODE_NOT_PDU_DT,
    ERR_ISO_CONNECT_CODE_NOT_PDU_CC,

    ERR_MCS = 5000,
    ERR_MCS_RECV_VER_NOT_3,
    ERR_MCS_RECV_ID_NOT_MCS_SDIN,
    ERR_MCS_APPID_IS_MCS_DPUM,
    ERR_MCS_APPID_NOT_MCS_SDRQ,
    ERR_MCS_BER_PARSE_HEADER_VAL_NOT_MATCH,
    ERR_MCS_BER_PARSE_HEADER_ERROR_CHECKING_STREAM,
    ERR_MCS_BER_HEADER_UNEXPECTED_TAG,
    ERR_MCS_BER_HEADER_TRUNCATED,
    ERR_MCS_PARSE_DOMAIN_PARAMS_ERROR_CHECKING_STREAM,
    ERR_MCS_RECV_AUCF_OPCODE_NOT_OK,
    ERR_MCS_RECV_AUCF_RES_NOT_0,
    ERR_MCS_RECV_AUCF_ERROR_CHECKING_STREAM,
    ERR_MCS_RECV_CJCF_OPCODE_NOT_CJCF,
    ERR_MCS_RECV_CJCF_EMPTY,
    ERR_MCS_RECV_CJCF_ERROR_CHECKING_STREAM,
    ERR_MCS_RECV_CONNECTION_REP_RES_NOT_0,
    ERR_MCS_RECV_CONNECTION_REP_ERROR_CHECKING_STREAM,
    ERR_MCS_RECV_CONNECT_INITIAL_TRUNCATED,
    ERR_MCS_RECV_EDQR_APPID_NOT_EDRQ,
    ERR_MCS_RECV_EDQR_TRUNCATED,
    ERR_MCS_RECV_AURQ_APPID_NOT_AURQ,
    ERR_MCS_RECV_AURQ_TRUNCATED,
    ERR_MCS_RECV_CJRQ_APPID_NOT_CJRQ,
    ERR_MCS_RECV_CJRQ_TRUNCATED,
    ERR_MCS_CHANNEL_NOT_FOUND,
    ERR_MCS_DATA_SHORT_HEADER,
    ERR_MCS_BAD_USERID,
    ERR_MCS_BAD_CHANID,
    ERR_MCS_PDU_TRUNCATED,
    ERR_MCS_INFOPACKET_TRUNCATED,
    ERR_MCS_PDU_TRAILINGDATA,

    ERR_GCC = 5500,

    ERR_SEC = 6000,
    ERR_SEC_PARSE_PUB_KEY_MAGIC_NOT_OK,
    ERR_SEC_PARSE_PUB_KEY_MODUL_NOT_OK,
    ERR_SEC_PARSE_PUB_KEY_ERROR_CHECKING_STREAM,
    ERR_SEC_PARSE_CRYPT_INFO_CERT_NOK,
    ERR_SEC_PARSE_CRYPT_INFO_CACERT_NULL,
    ERR_SEC_PARSE_CRYPT_INFO_CACERT_NOT_LOADED,
    ERR_SEC_PARSE_CRYPT_INFO_CACERT_NOT_MATCH,
    ERR_SEC_PARSE_CRYPT_INFO_X509_NOT_PARSED,
    ERR_SEC_PARSE_CRYPT_INFO_MOD_SIZE_NOT_OK,
    ERR_SEC_PARSE_CRYPT_INFO_RSA_EXP_NOT_OK,
    ERR_SEC_PARSE_CRYPT_INFO_ERROR_CHECKING_STREAM,
    ERR_SEC_PARSE_PUB_SIG_LEN_NOT_OK,
    ERR_SEC_PROCESS_LOGON_UNKNOWN_FLAGS,
    ERR_SEC_READING_RSAKEYS_INI_FAILED,
    ERR_SEC_UNEXPECTED_LICENSE_NEGOTIATION_PDU,
    ERR_SEC_EXPECTED_LICENSE_NEGOTIATION_PDU,
    ERR_SEC_EXPECTING_CLIENT_RANDOM,
    ERR_SEC_EXPECTED_LOGON_INFO,
    ERR_SEC_EXPECTED_LICENSE_NEG,
    ERR_SEC_PARSE_CRYPT_INFO_ENCRYPTION_REQUIRED,
    ERR_SEC_PARSE_CRYPT_INFO_BAD_RANDOM_LEN,
    ERR_SEC_PARSE_CRYPT_INFO_BAD_RSA_LEN,
    ERR_SEC_PARSE_CRYPT_INFO_UNIMPLEMENTED_TAG,
    ERR_SEC_PARSE_BAD_TAG_SIG_LEN,
    ERR_SEC_EXPECTING_512_BITS_CLIENT_RANDOM,

    ERR_LIC = 6500,

    ERR_CHANNEL_UNKNOWN_CHANNEL = 7000,
    ERR_CHANNEL_SESSION_CALLBACK_NOT_INITIALIZED,
    ERR_CHANNEL_SESSION_CALLBACK_FAILED,

    ERR_RDP_RDP_INIT = 8000,
    ERR_RDP_CHAN_INIT,
    ERR_RDP_SEND_LOGIN_SEC_INIT_NOK,
    ERR_RDP_SEND_INPUT_INIT_DATA_NOK,
    ERR_RDP_SEND_INVALIDATE_INIT_DATA_NOK,
    ERR_RDP_CHECK_FILE_INDEX_NOK,
    ERR_RDP_SEND_REDIR_CHANNEL_NOT_INIT,
    ERR_RDP_PROCESS_COLOR_POINTER_CACHE_NOT_OK,
    ERR_RDP_PROCESS_COLOR_POINTER_LEN_NOT_OK,
    ERR_RDP_PROCESS_POINTER_CACHE_LESS_0,
    ERR_RDP_PROCESS_POINTER_CACHE_NOT_OK,
    ERR_RDP_PROCESS_NEW_POINTER_CACHE_NOT_OK,
    ERR_RDP_PROCESS_NEW_POINTER_LEN_NOT_OK,
    ERR_RDP_SEND_CONFIRM_ACT_SEC_INIT_NOK,
    ERR_RDP_REC_WRITE_ITEM_NOT_FD,
    ERR_RDP_EXPECTING_DEMANDACTIVEPDU,
    ERR_RDP_UNEXPECTED_DEMANDACTIVEPDU,
    ERR_RDP_UNEXPECTED_VIRTUAL_CHANNEL,
    ERR_RDP_RESIZE_NOT_AVAILABLE,
    ERR_RDP_SLOWPATH,
    ERR_RDP_FASTPATH,
    ERR_RDP_DATA_TRUNCATED,

    ERR_WM_PASSWORD = 9000,
    ERR_WM_USERNAME,

    ERR_VNC = 10000,
    ERR_VNC_UNEXPECTED_ENCODING_IN_LIB_FRAME_BUFFER,
    ERR_VNC_MOD_SIGNAL_UNKNOWN_TYPE,
    ERR_VNC_BAD_BPP,
    ERR_VNC_NO_IP_SET,
    ERR_VNC_CONNECTION_ERROR,
    ERR_VNC_MEMORY_ALLOCATION_FAILED,
    ERR_VNC_OLDER_RDP_CLIENT_CANT_RESIZE,

    ERR_XUP_BAD_BPP = 11000,

    ERR_SERVER_BEGIN_UPDATE = 12000,
    ERR_SERVER_PALETTE,
    ERR_SERVER_PAINT_RECT,
    ERR_SERVER_SCREEN_BLT,
    ERR_SERVER_SET_POINTER,
    ERR_SERVER_SEND_TO_CHANNEL,
    ERR_SERVER_END_UPDATE,

    ERR_SERVER_RDP_RECV_ERR,

    ERR_SESSION_UNKNOWN_BACKEND = 13000,

    ERR_WAIT_OBJ_SOCKET = 14000,

    ERR_ORDERS_FORCE_SEND_FAILED = 15000,

    ERR_RECORDER_NO_OUTPUT_CODEC = 16000,
    ERR_RECORDER_FAILED_ALLOCATING_OUTPUT_MEDIA_CONTEXT,
    ERR_RECORDER_FAILED_TO_OPEN_CODEC,
    ERR_RECORDER_CODEC_NOT_FOUND,
    ERR_RECORDER_FAILED_TO_ALLOCATE_PICTURE,
    ERR_RECORDER_FAILED_TO_ALLOCATE_YUV420P_TEMPORARY_PICTURE,
    ERR_RECORDER_INVALID_OUTPUT_FORMAT_PARAMETER,
    ERR_RECORDER_FAILED_TO_OPEN_TARGET_FILE,
    ERR_RECORDER_FAILED_TO_ALLOC_STREAM,
    ERR_RECORDER_FAILED_TO_ALLOCATE_PICTURE_BUF,
    ERR_RECORDER_FAILED_TO_ALLOCATE_FRAME,
    ERR_RECORDER_FAILED_TO_ALLOCATE_FRAME_BUFFER,
    ERR_RECORDER_FAILED_TO_INITIALIZE_CONVERSION_CONTEXT,
    ERR_RECORDER_FAILED_TO_WRITE_RAW_FRAME,
    ERR_RECORDER_FAILED_TO_WRITE_ENCODED_FRAME,
    ERR_RECORDER_FAILED_TO_FOUND_PATH,
    ERR_RECORDER_ALLOCATION_FAILED,
    ERR_RECORDER_FRAME_ALLOCATION_FAILED,
    ERR_RECORDER_EMPTY_IMAGE,
    ERR_RECORDER_IMAGE_ALLOCATION_FAILED,
    ERR_RECORDER_INIT_FAILED,
    ERR_RECORDER_FILE_CRYPTED,

    ERR_RECORDER_SNAPSHOT_FAILED,

    ERR_BITMAP_LOAD_FAILED = 17000,

    ERR_BITMAP_CACHE = 18000,
    ERR_BITMAP_CACHE_TOO_BIG,
    ERR_BITMAP_UNSUPPORTED_COLOR_DEPTH,
    ERR_BITMAP_DECOMPRESSED_DATA_TOO_LARGE,
    ERR_BITMAP_8BIT_COLOR_DEPTH_MISSING_PALETTE,

    ERR_NEGO_SSL_REQUIRED_BY_SERVER = 19000,
    ERR_NEGO_SSL_NOT_ALLOWED_BY_SERVER,
    ERR_NEGO_SSL_CERT_NOT_ON_SERVER,
    ERR_NEGO_INCONSISTENT_FLAGS,
    ERR_NEGO_HYBRID_REQUIRED_BY_SERVER,

    ERR_WRM = 20000,
    ERR_WRM_INVALID_FILE,
    ERR_NATIVE_CAPTURE_OPEN_FAILED,
    ERR_NATIVE_CAPTURE_ZIP_COMPRESS,

    ERR_CIPHER_START,
    ERR_CIPHER_UPDATE,

    ERR_RECORDER_META_REFERENCE_WRM,

    ERR_WRM_RECORDER_OPEN_FAILED,
    ERR_WRM_RECORDER_ZIP_UNCOMPRESS,
    ERR_WRM_CHUNK_META_NOT_FOUND,
    ERR_WRM_INVALID_META_CHUNK,
    ERR_WRM_IDX_NOT_FOUND,
    ERR_WRM_INVALID_INIT_CRYPT,
    ERR_WRM_FAILED_OPENING_META_FILENAME,
};

class Error {
    public:
    int id;
    int errnum;
    char errstr[256];

    Error(int id, int errnum = 0)
    {
        this->id = id;
        this->errnum = errnum;
//        LOG(LOG_ERR, "error=%u", this->id);
//        exit(0);
    }

    const char * errmsg() {
        switch(this->id) {
        case NO_ERROR :
            snprintf(errstr, sizeof(errstr), "No error");
            break;
        case ERR_TRANSPORT_OPEN_FAILED :
            snprintf(errstr, sizeof(errstr), "Open file failed");
            break;
        case ERR_SESSION_UNKNOWN_BACKEND :
            snprintf(errstr, sizeof(errstr), "Unknown Backend");
            break;
        default:
            snprintf(errstr, sizeof(errstr), "Exception Error no : %d", this->id);
        }
        return this->errstr;
    }
};

#endif
