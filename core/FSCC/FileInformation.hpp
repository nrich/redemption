/*
    This program is free software; you can redistribute it and/or modify it
     under the terms of the GNU General Public License as published by the
     Free Software Foundation; either version 2 of the License, or (at your
     option) any later version.

    This program is distributed in the hope that it will be useful, but
     WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
     Public License for more details.

    You should have received a copy of the GNU General Public License along
     with this program; if not, write to the Free Software Foundation, Inc.,
     675 Mass Ave, Cambridge, MA 02139, USA.

    Product name: redemption, a FLOSS RDP proxy
    Copyright (C) Wallix 2015
    Author(s): Christophe Grosjean, Raphael Zhou
*/

#ifndef _REDEMPTION_CORE_FSCC_FILEINFORMATION_HPP_
#define _REDEMPTION_CORE_FSCC_FILEINFORMATION_HPP_

#include "stream.hpp"

namespace fscc {

// [MS-FSCC] - 2.3 FSCTL Structures
// ================================

// A process invokes an FSCTL on a handle to perform an action against the
//  file or directory associated with the handle. When a server receives an
//  FSCTL request, it SHOULD use the information in the request, which
//  includes a handle and, optionally, an input data buffer, to perform the
//  requested action. How a server performs the action requested by an FSCTL
//  is implementation-dependent.<12>

// The following table specifies the system-defined generic FSCTLs that are
//  permitted to be invoked across the network. Generic FSCTLs are used by
//  the local file systems or by multiple components within the system. Any
//  application, service, or driver may define private FSCTLs. Most private
//  FSCTLs are used locally in the internal driver stacks and do not flow
//  over the wire. However, if a component allows its private FSCTLs to flow
//  over the wire, that component is responsible for ensuring the FSCTLs and
//  associated data structures are documented. Examples of such private
//  FSCTLs can be found in [MS-SMB2] and [MS-DFSC].

//  +-----------------------------------------+-----------------------+
//  | FSCTL name                              | FSCTL function number |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_CREATE_OR_GET_OBJECT_ID           | 0x900c0               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_DELETE_OBJECT_ID                  | 0x900a0               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_DELETE_REPARSE_POINT              | 0x900ac               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_FILE_LEVEL_TRIM                   | 0x98208               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_FILESYSTEM_GET_STATISTICS         | 0x90060               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_FIND_FILES_BY_SID                 | 0x9008f               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_GET_COMPRESSION                   | 0x9003c               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_GET_INTEGRITY_INFORMATION         | 0x9027c               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_GET_NTFS_VOLUME_DATA              | 0x90064               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_GET_REFS_VOLUME_DATA              | 0x902D8               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_GET_OBJECT_ID                     | 0x9009c               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_GET_REPARSE_POINT                 | 0x900a8               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_GET_RETRIEVAL_POINTERS            | 0x90073               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_IS_PATHNAME_VALID                 | 0x9002c               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_LMR_SET_LINK_TRACKING_INFORMATION | 0x1400ec              |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_OFFLOAD_READ                      | 0x94264               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_OFFLOAD_WRITE                     | 0x98268               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_PIPE_PEEK                         | 0x11400c              |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_PIPE_TRANSCEIVE                   | 0x11c017              |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_PIPE_WAIT                         | 0x110018              |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_QUERY_ALLOCATED_RANGES            | 0x940cf               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_QUERY_FAT_BPB                     | 0x90058               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_QUERY_FILE_REGIONS                | 0x90284               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_QUERY_ON_DISK_VOLUME_INFO         | 0x9013c               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_QUERY_SPARING_INFO                | 0x90138               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_READ_FILE_USN_DATA                | 0x900eb               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_RECALL_FILE                       | 0x90117               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_SET_COMPRESSION                   | 0x9c040               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_SET_DEFECT_MANAGEMENT             | 0x98134               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_SET_ENCRYPTION                    | 0x900D7               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_SET_INTEGRITY_INFORMATION         | 0x9C280               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_SET_OBJECT_ID                     | 0x90098               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_SET_OBJECT_ID_EXTENDED            | 0x900bc               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_SET_REPARSE_POINT                 | 0x900a4               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_SET_SPARSE                        | 0x900c4               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_SET_ZERO_DATA                     | 0x980c8               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_SET_ZERO_ON_DEALLOCATION          | 0x90194               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_SIS_COPYFILE                      | 0x90100               |
//  +-----------------------------------------+-----------------------+
//  | FSCTL_WRITE_USN_CLOSE_RECORD            | 0x900ef               |
//  +-----------------------------------------+-----------------------+

enum {
      FSCTL_CREATE_OR_GET_OBJECT_ID           = 0x900c0
    , FSCTL_DELETE_OBJECT_ID                  = 0x900a0
    , FSCTL_DELETE_REPARSE_POINT              = 0x900ac
    , FSCTL_FILE_LEVEL_TRIM                   = 0x98208
    , FSCTL_FILESYSTEM_GET_STATISTICS         = 0x90060
    , FSCTL_FIND_FILES_BY_SID                 = 0x9008f
    , FSCTL_GET_COMPRESSION                   = 0x9003c
    , FSCTL_GET_INTEGRITY_INFORMATION         = 0x9027c
    , FSCTL_GET_NTFS_VOLUME_DATA              = 0x90064
    , FSCTL_GET_REFS_VOLUME_DATA              = 0x902D8
    , FSCTL_GET_OBJECT_ID                     = 0x9009c
    , FSCTL_GET_REPARSE_POINT                 = 0x900a8
    , FSCTL_GET_RETRIEVAL_POINTERS            = 0x90073
    , FSCTL_IS_PATHNAME_VALID                 = 0x9002c
    , FSCTL_LMR_SET_LINK_TRACKING_INFORMATION = 0x1400ec
    , FSCTL_OFFLOAD_READ                      = 0x94264
    , FSCTL_OFFLOAD_WRITE                     = 0x98268
    , FSCTL_PIPE_PEEK                         = 0x11400c
    , FSCTL_PIPE_TRANSCEIVE                   = 0x11c017
    , FSCTL_PIPE_WAIT                         = 0x110018
    , FSCTL_QUERY_ALLOCATED_RANGES            = 0x940cf
    , FSCTL_QUERY_FAT_BPB                     = 0x90058
    , FSCTL_QUERY_FILE_REGIONS                = 0x90284
    , FSCTL_QUERY_ON_DISK_VOLUME_INFO         = 0x9013c
    , FSCTL_QUERY_SPARING_INFO                = 0x90138
    , FSCTL_READ_FILE_USN_DATA                = 0x900eb
    , FSCTL_RECALL_FILE                       = 0x90117
    , FSCTL_SET_COMPRESSION                   = 0x9c040
    , FSCTL_SET_DEFECT_MANAGEMENT             = 0x98134
    , FSCTL_SET_ENCRYPTION                    = 0x900D7
    , FSCTL_SET_INTEGRITY_INFORMATION         = 0x9C280
    , FSCTL_SET_OBJECT_ID                     = 0x90098
    , FSCTL_SET_OBJECT_ID_EXTENDED            = 0x900bc
    , FSCTL_SET_REPARSE_POINT                 = 0x900a4
    , FSCTL_SET_SPARSE                        = 0x900c4
    , FSCTL_SET_ZERO_DATA                     = 0x980c8
    , FSCTL_SET_ZERO_ON_DEALLOCATION          = 0x90194
    , FSCTL_SIS_COPYFILE                      = 0x90100
    , FSCTL_WRITE_USN_CLOSE_RECORD            = 0x900ef
};

// [MS-FSCC] - 2.4.6 FileAttributeTagInformation
// =============================================

// This information class is used to query for attribute and reparse tag
//  information for a file.

// A FILE_ATTRIBUTE_TAG_INFORMATION data element, defined as follows, is
//  returned by the server.

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | | | | | | | | | | |1| | | | | | | | | |2| | | | | | | | | |3| |
// |0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                         FileAttributes                        |
// +---------------------------------------------------------------+
// |                           ReparseTag                          |
// +---------------------------------------------------------------+

// FileAttributes (4 bytes): A 32-bit unsigned integer that contains the file
//  attributes. Valid file attributes are as specified in section 2.6.

// ReparseTag (4 bytes): A 32-bit unsigned integer that specifies the reparse
//  point tag. If the FileAttributes member includes the
//  FILE_ATTRIBUTE_REPARSE_POINT attribute flag, this member specifies the
//  reparse tag. Otherwise, this member SHOULD be set to 0, and MUST be
//  ignored. Section 2.1.2.1 contains more details on reparse tags.

// This operation returns a status code, as specified in [MS-ERREF] section
//  2.3. The status code returned directly by the function that processes
//  this file information class MUST be STATUS_SUCCESS or one of the
//  following.

//  +-----------------------------+--------------------------------------------+
//  | Error code                  | Meaning                                    |
//  +-----------------------------+--------------------------------------------+
//  | STATUS_INFO_LENGTH_MISMATCH | The specified information record length    |
//  | 0xC0000004                  | does not match the length that is required |
//  |                             | for the specified information class.       |
//  +-----------------------------+--------------------------------------------+
//  | STATUS_ACCESS_DENIED        | The handle was not opened to read file     |
//  | 0xC0000022                  | data or file attributes.                   |
//  +-----------------------------+--------------------------------------------+

class FileAttributeTagInformation {
    uint32_t FileAttributes = 0;
    uint32_t ReparseTag     = 0;

public:
    FileAttributeTagInformation() = default;

    FileAttributeTagInformation(uint32_t FileAttributes, uint32_t ReparseTag)
    : FileAttributes(FileAttributes)
    , ReparseTag(ReparseTag) {}

    inline void emit(Stream & stream) const {
        stream.out_uint32_le(this->FileAttributes);
        stream.out_uint32_le(this->ReparseTag);
    }

    inline void receive(Stream & stream) {
        {
            const unsigned expected = 8;    // FileAttributes(4) + ReparseTag(4)

            if (!stream.in_check_rem(expected)) {
                LOG(LOG_ERR,
                    "Truncated FileAttributeTagInformation: expected=%u remains=%u",
                    expected, stream.in_remain());
                throw Error(ERR_FSCC_DATA_TRUNCATED);
            }
        }

        this->FileAttributes = stream.in_uint32_le();
        this->ReparseTag     = stream.in_uint32_le();
    }

    inline static size_t size() {
        return 8;   /* FileAttributes(4) + ReparseTag(4) */
    }

private:
    size_t str(char * buffer, size_t size) const {
        size_t length = ::snprintf(buffer, size,
            "FileAttributeTagInformation: FileAttributes=0x%X ReparseTag=%u",
            this->FileAttributes, this->ReparseTag);
        return ((length < size) ? length : size - 1);
    }

public:
    inline void log(int level) const {
        char buffer[2048];
        this->str(buffer, sizeof(buffer));
        buffer[sizeof(buffer) - 1] = 0;
        LOG(level, buffer);
    }
};


// [MS-FSCC] - 2.4.7 FileBasicInformation
// ======================================

// This information class is used to query or set file information.

// A FILE_BASIC_INFORMATION data element, defined as follows, is returned by
//  the server or provided by the client.

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | | | | | | | | | | |1| | | | | | | | | |2| | | | | | | | | |3| |
// |0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                          CreationTime                         |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                         LastAccessTime                        |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                         LastWriteTime                         |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                           ChangeTime                          |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                         FileAttributes                        |
// +---------------------------------------------------------------+
// |                            Reserved                           |
// +---------------------------------------------------------------+

// CreationTime (8 bytes): The time when the file was created; see section
//  2.1.1. A valid time for this field is an integer greater than or equal to
//  0. When setting file attributes, a value of 0 indicates to the server
//  that it MUST NOT change this attribute. When setting file attributes, a
//  value of -1 indicates to the server that it MUST NOT change this
//  attribute for all subsequent operations on the same file handle. This
//  field MUST NOT be set to a value less than -1.

// LastAccessTime (8 bytes): The last time the file was accessed; see section
//  2.1.1. A valid time for this field is an integer greater than or equal to
//  0. When setting file attributes, a value of 0 indicates to the server
//  that it MUST NOT change this attribute. When setting file attributes, a
//  value of -1 indicates to the server that it MUST NOT change this
//  attribute for all subsequent operations on the same file handle. This
//  field MUST NOT be set to a value less than -1.<89>

// LastWriteTime (8 bytes): The last time information was written to the
//  file; see section 2.1.1. A valid time for this field is an integer
//  greater than or equal to 0. When setting file attributes, a value of 0
//  indicates to the server that it MUST NOT change this attribute. When
//  setting file attributes, a value of -1 indicates to the server that it
//  MUST NOT change this attribute for all subsequent operations on the same
//  file handle. This field MUST NOT be set to a value less than -1.<90>

// ChangeTime (8 bytes): The last time the file was changed; see section
//  2.1.1. A valid time for this field is an integer greater than or equal to
//  0. When setting file attributes, a value of 0 indicates to the server
//  that it MUST NOT change this attribute. When setting file attributes, a
//  value of -1 indicates to the server that it MUST NOT change this
//  attribute for all subsequent operations on the same file handle. This
//  field MUST NOT be set to a value less than -1.<91>

// FileAttributes (4 bytes): A 32-bit unsigned integer that contains the file
//  attributes. Valid file attributes are specified in section 2.6.

// Reserved (4 bytes): A 32-bit field. This field is reserved. This field can
//  be set to any value, and MUST be ignored.

// This operation returns a status code, as specified in [MS-ERREF] section
//  2.3. The status code returned directly by the function that processes
//  this file information class MUST be STATUS_SUCCESS or one of the
//  following.

//  +-----------------------------+--------------------------------------------+
//  | Error code                  | Meaning                                    |
//  +-----------------------------+--------------------------------------------+
//  | STATUS_INFO_LENGTH_MISMATCH | The specified information record length    |
//  | 0xC0000004                  | does not match the length that is required |
//  |                             | for the specified information class.       |
//  +-----------------------------+--------------------------------------------+
//  | STATUS_ACCESS_DENIED        | The handle was not opened to read file     |
//  | 0xC0000022                  | data or file attributes.                   |
//  +-----------------------------+--------------------------------------------+

class FileBasicInformation {
    uint64_t CreationTime   = 0;
    uint64_t LastAccessTime = 0;
    uint64_t LastWriteTime  = 0;
    uint64_t ChangeTime     = 0;
    uint32_t FileAttributes = 0;

public:
    FileBasicInformation() = default;

    FileBasicInformation(uint64_t CreationTime, uint64_t LastAccessTime,
                         uint64_t LastWriteTime, uint64_t ChangeTime,
                         uint32_t FileAttributes)
    : CreationTime(CreationTime)
    , LastAccessTime(LastAccessTime)
    , LastWriteTime(LastWriteTime)
    , ChangeTime(ChangeTime)
    , FileAttributes(FileAttributes) {}

    inline void emit(Stream & stream) const {
        stream.out_uint64_le(this->CreationTime);
        stream.out_uint64_le(this->LastAccessTime);
        stream.out_uint64_le(this->LastWriteTime);
        stream.out_uint64_le(this->ChangeTime);

        stream.out_uint32_le(this->FileAttributes);

        // Reserved(4), MUST NOT be transmitted.
    }

    inline void receive(Stream & stream) {
        {
            const unsigned expected = 36;   // CreationTime(8) + LastAccessTime(8) +
                                            //     LastWriteTime(8) + ChangeTime(8) +
                                            //     FileAttributes(4)

            if (!stream.in_check_rem(expected)) {
                LOG(LOG_ERR,
                    "Truncated FileBasicInformation: expected=%u remains=%u",
                    expected, stream.in_remain());
                throw Error(ERR_FSCC_DATA_TRUNCATED);
            }
        }

        this->CreationTime   = stream.in_uint64_le();
        this->LastAccessTime = stream.in_uint64_le();
        this->LastWriteTime  = stream.in_uint64_le();
        this->ChangeTime     = stream.in_uint64_le();

        this->FileAttributes = stream.in_uint32_le();

        // Reserved(4), MUST NOT be transmitted.
    }

    inline static size_t size() {
        return 36;  /* CreationTime(8) + LastAccessTime(8) + LastWriteTime(8) + ChangeTime(8) + FileAttributes(4) */
    }

private:
    size_t str(char * buffer, size_t size) const {
        size_t length = ::snprintf(buffer, size,
            "FileBasicInformation: CreationTime=%" PRIu64 " LastAccessTime=%" PRIu64
                " LastWriteTime=%" PRIu64 " ChangeTime=%" PRIu64 " FileAttributes=0x%X",
            this->CreationTime, this->LastAccessTime, this->LastWriteTime,
            this->ChangeTime, this->FileAttributes);
        return ((length < size) ? length : size - 1);
    }

public:
    inline void log(int level) const {
        char buffer[2048];
        this->str(buffer, sizeof(buffer));
        buffer[sizeof(buffer) - 1] = 0;
        LOG(level, buffer);
    }
};  // FileBasicInformation

// [MS-FSCC] - 2.4.8 FileBothDirectoryInformation
// ==============================================

// This information class is used in directory enumeration to return detailed
//  information about the contents of a directory.

// This information class returns a list that contains a
//  FILE_BOTH_DIR_INFORMATION data element for each file or directory within
//  the target directory. This list MUST reflect the presence of a
//  subdirectory named "." (synonymous with the target directory itself)
//  within the target directory and one named ".." (synonymous with the
//  parent directory of the target directory). For more details, see section
//  2.1.5.1.

// This information class differs from FileDirectoryInformation (section
//  2.4.10) in that it includes short names in the returns list.

// When multiple FILE_BOTH_DIR_INFORMATION data elements are present in the
//  buffer, each MUST be aligned on an 8-byte boundary. Any bytes inserted
//  for alignment SHOULD be set to zero, and the receiver MUST ignore them.
//  No padding is required following the last data element.

// A FILE_BOTH_DIR_INFORMATION data element is as follows.

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | | | | | | | | | | |1| | | | | | | | | |2| | | | | | | | | |3| |
// |0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                        NextEntryOffset                        |
// +---------------------------------------------------------------+
// |                           FileIndex                           |
// +---------------------------------------------------------------+
// |                          CreationTime                         |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                         LastAccessTime                        |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                         LastWriteTime                         |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                           ChangeTime                          |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                           EndOfFile                           |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                         AllocationSize                        |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                         FileAttributes                        |
// +---------------------------------------------------------------+
// |                         FileNameLength                        |
// +---------------------------------------------------------------+
// |                             EaSize                            |
// +---------------+---------------+-------------------------------+
// |ShortNameLength|    Reserved   |           ShortName           |
// +---------------+---------------+-------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                              ...                              |
// +-------------------------------+-------------------------------+
// |              ...              |      FileName (variable)      |
// +-------------------------------+-------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+

// NextEntryOffset (4 bytes): A 32-bit unsigned integer that contains the
//  byte offset from the beginning of this entry, at which the next
//  FILE_BOTH_DIR_INFORMATION entry is located, if multiple entries are
//  present in a buffer. This member is zero if no other entries follow this
//  one. An implementation MUST use this value to determine the location of
//  the next entry (if multiple entries are present in a buffer).

// FileIndex (4 bytes): A 32-bit unsigned integer that contains the byte
//  offset of the file within the parent directory. For file systems in which
//  the position of a file within the parent directory is not fixed and can
//  be changed at any time to maintain sort order, this field SHOULD be set
//  to 0x00000000 and MUST be ignored.<92>

// CreationTime (8 bytes): The time when the file was created; see section
//  2.1.1. This value MUST be greater than or equal to 0.

// LastAccessTime (8 bytes): The last time the file was accessed; see section
//  2.1.1. This value MUST be greater than or equal to 0.

// LastWriteTime (8 bytes): The last time information was written to the
//  file; see section 2.1.1. This value MUST be greater than or equal to 0.

// ChangeTime (8 bytes): The last time the file was changed; see section
//  2.1.1. This value MUST be greater than or equal to 0.

// EndOfFile (8 bytes): A 64-bit signed integer that contains the absolute
//  new end-of-file position as a byte offset from the start of the file.
//  EndOfFile specifies the offset to the byte immediately following the last
//  valid byte in the file. Because this value is zero-based, it actually
//  refers to the first free byte in the file. That is, it is the offset from
//  the beginning of the file at which new bytes appended to the file will be
//  written. The value of this field MUST be greater than or equal to 0.

// AllocationSize (8 bytes): A 64-bit signed integer that contains the file
//  allocation size, in bytes. The value of this field MUST be an integer
//  multiple of the cluster size.

// FileAttributes (4 bytes): A 32-bit unsigned integer that contains the file
//  attributes. Valid file attributes are specified in section 2.6.

// FileNameLength (4 bytes): A 32-bit unsigned integer that specifies the
//  length, in bytes, of the file name contained within the FileName member.

// EaSize (4 bytes): A 32-bit unsigned integer that contains the combined
//  length, in bytes, of the extended attributes (EA) for the file.

// ShortNameLength (1 byte): An 8-bit signed integer that specifies the
//  length, in bytes, of the file name contained in the ShortName member.
//  This value MUST be greater than or equal to 0.

// Reserved (1 byte): Reserved for alignment. This field can contain any
//  value and MUST be ignored.

// ShortName (24 bytes): A sequence of Unicode characters containing the
//  short (8.3) file name. When working with this field, use ShortNameLength
//  to determine the length of the file name rather than assuming the
//  presence of a trailing null delimiter.

// FileName (variable): A sequence of Unicode characters containing the file
//  name. When working with this field, use FileNameLength to determine the
//  length of the file name rather than assuming the presence of a trailing
//  null delimiter. Dot directory names are valid for this field. For more
//  details, see section 2.1.5.1.

// This operation returns a status code, as specified in [MS-ERREF] section
//  2.3. The status code returned directly by the function that processes
//  this file information class MUST be STATUS_SUCCESS or one of the
//  following.

//  +-----------------------------+--------------------------------------------+
//  | Error code                  | Meaning                                    |
//  +-----------------------------+--------------------------------------------+
//  | STATUS_INFO_LENGTH_MISMATCH | The specified information record length    |
//  | 0xC0000004                  | does not match the length that is required |
//  |                             | for the specified information class.       |
//  +-----------------------------+--------------------------------------------+

class FileBothDirectoryInformation {
    uint32_t NextEntryOffset = 0;
    uint32_t FileIndex       = 0;
    uint64_t CreationTime    = 0;
    uint64_t LastAccessTime  = 0;
    uint64_t LastWriteTime   = 0;
    uint64_t ChangeTime      = 0;
    int64_t  EndOfFile       = 0;
    int64_t  AllocationSize  = 0;
    uint32_t FileAttributes  = 0;
    uint32_t EaSize          = 0;

    std::string short_name;
    std::string file_name;

public:
    FileBothDirectoryInformation() = default;

    FileBothDirectoryInformation(uint64_t CreationTime, uint64_t LastAccessTime,
                         uint64_t LastWriteTime, uint64_t ChangeTime,
                         int64_t EndOfFile, int64_t AllocationSize,
                         uint32_t FileAttributes, const char * file_name)
    : CreationTime(CreationTime)
    , LastAccessTime(LastAccessTime)
    , LastWriteTime(LastWriteTime)
    , ChangeTime(ChangeTime)
    , EndOfFile(EndOfFile)
    , AllocationSize(AllocationSize)
    , FileAttributes(FileAttributes)
    , file_name(file_name) {}

    inline void emit(Stream & stream) const {
        stream.out_uint32_le(this->NextEntryOffset);
        stream.out_uint32_le(this->FileIndex);

        stream.out_uint64_le(this->CreationTime);
        stream.out_uint64_le(this->LastAccessTime);
        stream.out_uint64_le(this->LastWriteTime);
        stream.out_uint64_le(this->ChangeTime);

        stream.out_sint64_le(this->EndOfFile);
        stream.out_sint64_le(this->AllocationSize);

        stream.out_uint32_le(this->FileAttributes);

        // The null-terminator is included.
        const size_t maximum_length_of_FileName_in_bytes = (this->file_name.length() + 1) * 2;

        uint8_t * const FileName_unicode_data = static_cast<uint8_t *>(::alloca(
                    maximum_length_of_FileName_in_bytes));
        size_t size_of_FileName_unicode_data = ::UTF8toUTF16(
            reinterpret_cast<const uint8_t *>(this->file_name.c_str()), FileName_unicode_data,
            maximum_length_of_FileName_in_bytes);
        // Writes null terminator.
        FileName_unicode_data[size_of_FileName_unicode_data    ] =
        FileName_unicode_data[size_of_FileName_unicode_data + 1] = 0;
        size_of_FileName_unicode_data += 2;

        stream.out_uint32_le(size_of_FileName_unicode_data);

        stream.out_uint32_le(this->EaSize);

        const size_t maximum_length_of_ShortName_in_bytes = (this->short_name.length() + 1) * 2;

        uint8_t * const ShortName_unicode_data = static_cast<uint8_t *>(::alloca(
                    maximum_length_of_ShortName_in_bytes));
        size_t size_of_ShortName_unicode_data = ::UTF8toUTF16(
            reinterpret_cast<const uint8_t *>(this->short_name.c_str()), ShortName_unicode_data,
            maximum_length_of_ShortName_in_bytes);
        if (size_of_ShortName_unicode_data > 0) {
            // Writes null terminator.
            ShortName_unicode_data[size_of_ShortName_unicode_data    ] =
            ShortName_unicode_data[size_of_ShortName_unicode_data + 1] = 0;
            size_of_ShortName_unicode_data += 2;

            REDASSERT(size_of_ShortName_unicode_data <= 24 /* ShortName(24) */);
        }

        stream.out_sint8(size_of_ShortName_unicode_data);

        // Reserved(4), MUST NOT be transmitted.

        stream.out_copy_bytes(ShortName_unicode_data, size_of_ShortName_unicode_data);
        if (size_of_ShortName_unicode_data < 24  /* ShortName(24) */) {
            stream.out_clear_bytes(24 /* ShortName(24) */ - size_of_ShortName_unicode_data);
        }

        stream.out_copy_bytes(FileName_unicode_data, size_of_FileName_unicode_data);
    }

    inline void receive(Stream & stream) {
        {
            const unsigned expected = 93;   // NextEntryOffset(4) + FileIndex(4) +
                                            //     CreationTime(8) + LastAccessTime(8) +
                                            //     LastWriteTime(8) + ChangeTime(8) +
                                            //     EndOfFile(8) + AllocationSize(8) +
                                            //     FileAttributes(4) + FileNameLength(4) +
                                            //     EaSize(4) + ShortNameLength(1) +
                                            //     ShortName(24)
            if (!stream.in_check_rem(expected)) {
                LOG(LOG_ERR,
                    "Truncated FileBothDirectoryInformation (0): expected=%u remains=%u",
                    expected, stream.in_remain());
                throw Error(ERR_FSCC_DATA_TRUNCATED);
            }
        }

        this->NextEntryOffset = stream.in_uint32_le();
        this->FileIndex       = stream.in_uint32_le();
        this->CreationTime    = stream.in_uint64_le();
        this->LastAccessTime  = stream.in_uint64_le();
        this->LastWriteTime   = stream.in_uint64_le();
        this->ChangeTime      = stream.in_uint64_le();
        this->EndOfFile       = stream.in_sint64_le();
        this->AllocationSize  = stream.in_sint64_le();
        this->FileAttributes  = stream.in_uint32_le();

        const uint32_t FileNameLength = stream.in_uint32_le();

        this->EaSize = stream.in_uint32_le();

        const int8_t ShortNameLength = stream.in_sint8();

        // Reserved(1), MUST NOT be transmitted.

        uint8_t ShortName[24];

        stream.in_copy_bytes(ShortName, sizeof(ShortName));

        const size_t maximum_length_of_utf8_character_in_bytes = 4;

        const size_t size_of_ShortName_utf8_string =
            ShortNameLength / 2 * maximum_length_of_utf8_character_in_bytes + 1;
        uint8_t * const ShortName_utf8_string = static_cast<uint8_t *>(
            ::alloca(size_of_ShortName_utf8_string));
        const size_t length_of_ShortName_utf8_string = ::UTF16toUTF8(
            ShortName, ShortNameLength / 2, ShortName_utf8_string, size_of_ShortName_utf8_string);
        this->short_name.assign(::char_ptr_cast(ShortName_utf8_string),
            length_of_ShortName_utf8_string);

        {
            const unsigned expected = FileNameLength;   // FileName(variable)

            if (!stream.in_check_rem(expected)) {
                LOG(LOG_ERR,
                    "Truncated FileBothDirectoryInformation (1): expected=%u remains=%u",
                    expected, stream.in_remain());
                throw Error(ERR_RDPDR_PDU_TRUNCATED);
            }
        }

        uint8_t * const FileName_unicode_data = static_cast<uint8_t *>(::alloca(FileNameLength));

        stream.in_copy_bytes(FileName_unicode_data, FileNameLength);

        const size_t size_of_FileName_utf8_string =
            FileNameLength / 2 * maximum_length_of_utf8_character_in_bytes + 1;
        uint8_t * const FileName_utf8_string = static_cast<uint8_t *>(
            ::alloca(size_of_FileName_utf8_string));
        const size_t length_of_FileName_utf8_string = ::UTF16toUTF8(
            FileName_unicode_data, FileNameLength / 2, FileName_utf8_string, size_of_FileName_utf8_string);
        this->file_name.assign(::char_ptr_cast(FileName_utf8_string),
            length_of_FileName_utf8_string);
    }

    inline size_t size() const {
        size_t size = 93;   // NextEntryOffset(4) + FileIndex(4) +
                            //     CreationTime(8) + LastAccessTime(8) +
                            //     LastWriteTime(8) + ChangeTime(8) +
                            //     EndOfFile(8) + AllocationSize(8) +
                            //     FileAttributes(4) + FileNameLength(4) +
                            //     EaSize(4) + ShortNameLength(1) +
                            //     ShortName(24)

        // Reserved(1), MUST NOT be transmitted.

        // The null-terminator is included.
        const size_t maximum_length_of_FileName_in_bytes = (this->file_name.length() + 1) * 2;

        uint8_t * const unicode_data = static_cast<uint8_t *>(::alloca(
                    maximum_length_of_FileName_in_bytes));
        size_t size_of_unicode_data = ::UTF8toUTF16(
            reinterpret_cast<const uint8_t *>(this->file_name.c_str()), unicode_data,
            maximum_length_of_FileName_in_bytes);
        // Writes null terminator.
        unicode_data[size_of_unicode_data    ] =
        unicode_data[size_of_unicode_data + 1] = 0;
        size_of_unicode_data += 2;

        return size + size_of_unicode_data;
    }

private:
    size_t str(char * buffer, size_t size) const {
        size_t length = ::snprintf(buffer, size,
            "FileBothDirectoryInformation: NextEntryOffset=%u FileIndex=%u CreationTime=%" PRIu64
                " LastAccessTime=%" PRIu64 " LastWriteTime=%" PRIu64 " ChangeTime=%" PRIu64
                " EndOfFile=%" PRId64 " AllocationSize=%" PRId64 " FileAttributes=0x%X "
                "EaSize=%u ShortName=\"%s\" FileName=\"%s\"",
            this->NextEntryOffset, this->FileIndex,
            this->CreationTime, this->LastAccessTime, this->LastWriteTime,
            this->ChangeTime, this->EndOfFile, this->AllocationSize, this->FileAttributes,
            this->EaSize, this->short_name.c_str(), this->file_name.c_str());
        return ((length < size) ? length : size - 1);
    }

public:
    inline void log(int level) const {
        char buffer[2048];
        this->str(buffer, sizeof(buffer));
        buffer[sizeof(buffer) - 1] = 0;
        LOG(level, buffer);
    }
};  // FileBothDirectoryInformation

// [MS-FSCC] - 2.4.38 FileStandardInformation
// ==========================================

// This information class is used to query file information.

// A FILE_STANDARD_INFORMATION data element, defined as follows, is returned
//  by the server.

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | | | | | | | | | | |1| | | | | | | | | |2| | | | | | | | | |3| |
// |0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                         AllocationSize                        |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                           EndOfFile                           |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                         NumberOfLinks                         |
// +---------------+---------------+-------------------------------+
// | DeletePending |   Directory   |            Reserved           |
// +---------------+---------------+-------------------------------+

// AllocationSize (8 bytes): A 64-bit signed integer that contains the file
//  allocation size, in bytes. The value of this field MUST be an integer
//  multiple of the cluster size.

// EndOfFile (8 bytes): A 64-bit signed integer that contains the absolute
//  end-of-file position as a byte offset from the start of the file.
//  EndOfFile specifies the offset to the byte immediately following the last
//  valid byte in the file. Because this value is zero-based, it actually
//  refers to the first free byte in the file. That is, it is the offset from
//  the beginning of the file at which new bytes appended to the file will be
//  written. The value of this field MUST be greater than or equal to 0.

// NumberOfLinks (4 bytes): A 32-bit unsigned integer that contains the
//  number of non-deleted links to this file.

// DeletePending (1 byte): A Boolean (section 2.1.8) value. Set to TRUE to
//  indicate that a file deletion has been requested; set to FALSE otherwise.

// Directory (1 byte): A Boolean (section 2.1.8) value. Set to TRUE to
//  indicate that the file is a directory; set to FALSE otherwise.

// Reserved (2 bytes): A 16-bit field. This field is reserved. This field can
//  be set to any value, and MUST be ignored.

// This operation returns a status code, as specified in [MS-ERREF] section
//  2.3. The status code returned directly by the function that processes
//  this file information class MUST be STATUS_SUCCESS or one of the
//  following.

//  +-----------------------------+-----------------------------------------+
//  | Error code                  | Meaning                                 |
//  +-----------------------------+-----------------------------------------+
//  | STATUS_INFO_LENGTH_MISMATCH | The specified information record length |
//  | 0xC0000004                  | does not match the length that is       |
//  |                             | required for the specified information  |
//  |                             | class.                                  |
//  +-----------------------------+-----------------------------------------+

class FileStandardInformation {
    int64_t  AllocationSize = 0;
    int64_t  EndOfFile      = 0;
    uint32_t NumberOfLinks  = 0;
    uint8_t  DeletePending  = 0;
    uint8_t  Directory      = 0;

public:
    FileStandardInformation() = default;

    FileStandardInformation(int64_t AllocationSize, int64_t EndOfFile,
                            uint32_t NumberOfLinks, uint8_t DeletePending,
                            uint8_t Directory)
    : AllocationSize(AllocationSize)
    , EndOfFile(EndOfFile)
    , NumberOfLinks(NumberOfLinks)
    , DeletePending(DeletePending)
    , Directory(Directory) {}

    inline void emit(Stream & stream) const {
        stream.out_sint64_le(this->AllocationSize);
        stream.out_sint64_le(this->EndOfFile);

        stream.out_uint32_le(this->NumberOfLinks);

        stream.out_uint8(this->DeletePending);
        stream.out_uint8(this->Directory);

        // Reserved(2), MUST NOT be transmitted.
    }

    inline void receive(Stream & stream) {
        {
            const unsigned expected = 22;   // AllocationSize(8) + EndOfFile(8) +
                                            //     NumberOfLinks(4) + DeletePending(1) +
                                            //     Directory(1)
            if (!stream.in_check_rem(expected)) {
                LOG(LOG_ERR,
                    "Truncated FileStandardInformation: expected=%u remains=%u",
                    expected, stream.in_remain());
                throw Error(ERR_FSCC_DATA_TRUNCATED);
            }
        }

        this->AllocationSize = stream.in_sint64_le();
        this->EndOfFile      = stream.in_sint64_le();
        this->NumberOfLinks  = stream.in_uint32_le();
        this->DeletePending  = stream.in_uint8();
        this->Directory      = stream.in_uint8();
    }

    inline static size_t size() {
        return 22;  // AllocationSize(8) + EndOfFile(8) +
                    //     NumberOfLinks(4) + DeletePending(1) +
                    //     Directory(1)
    }

private:
    size_t str(char * buffer, size_t size) const {
        size_t length = ::snprintf(buffer, size,
            "FileBothDirectoryInformation: AllocationSize=%" PRId64
                " EndOfFile=%" PRId64 " NumberOfLinks=%u "
                "DeletePending=%u Directory=%u",
            this->AllocationSize, this->EndOfFile, this->NumberOfLinks,
            this->DeletePending, this->Directory);
        return ((length < size) ? length : size - 1);
    }

public:
    inline void log(int level) const {
        char buffer[2048];
        this->str(buffer, sizeof(buffer));
        buffer[sizeof(buffer) - 1] = 0;
        LOG(level, buffer);
    }
};  // FileStandardInformation

// [MS-FSCC] - 2.5.1 FileFsAttributeInformation
// ============================================

// This information class is used to query attribute information for a file
//  system.

// A FILE_FS_ATTRIBUTE_INFORMATION data element, defined as follows, is
//  returned by the server.

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | | | | | | | | | | |1| | | | | | | | | |2| | | | | | | | | |3| |
// |0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                      FileSystemAttributes                     |
// +---------------------------------------------------------------+
// |                   MaximumComponentNameLength                  |
// +---------------------------------------------------------------+
// |                      FileSystemNameLength                     |
// +---------------------------------------------------------------+
// |                   FileSystemName (variable)                   |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+

// FileSystemAttributes (4 bytes): A 32-bit unsigned integer that contains a
//  bitmask of flags that specify attributes of the specified file system as
//  a combination of the following flags. The value of this field MUST be a
//  bitwise OR of zero or more of the following with the exception that
//  FILE_FILE_COMPRESSION and FILE_VOLUME_IS_COMPRESSED cannot both be set.
//  Any flag values not explicitly mentioned here can be set to any value,
//  and MUST be ignored.

//  +-----------------------------------+--------------------------------------+
//  | Value                             | Meaning                              |
//  +-----------------------------------+--------------------------------------+
//  | FILE_SUPPORTS_USN_JOURNAL         | The file system implements a USN     |
//  | 0x02000000                        | change journal.<126>                 |
//  +-----------------------------------+--------------------------------------+
//  | FILE_SUPPORTS_OPEN_BY_FILE_ID     | The file system supports opening a   |
//  | 0x01000000                        | file by FileID or ObjectID.<127>     |
//  +-----------------------------------+--------------------------------------+
//  | FILE_SUPPORTS_EXTENDED_ATTRIBUTES | The file system persistently stores  |
//  | 0x00800000                        | Extended Attribute information per   |
//  |                                   | file.<128>                           |
//  +-----------------------------------+--------------------------------------+
//  | FILE_SUPPORTS_HARD_LINKS          | The file system supports hard        |
//  | 0x00400000                        | linking files.<129>                  |
//  +-----------------------------------+--------------------------------------+
//  | FILE_SUPPORTS_TRANSACTIONS        | The volume supports                  |
//  | 0x00200000                        | transactions.<130>                   |
//  +-----------------------------------+--------------------------------------+
//  | FILE_SEQUENTIAL_WRITE_ONCE        | The underlying volume is write once. |
//  | 0x00100000                        |                                      |
//  +-----------------------------------+--------------------------------------+
//  | FILE_READ_ONLY_VOLUME             | If set, the volume has been mounted  |
//  | 0x00080000                        | in read-only mode.<131>              |
//  +-----------------------------------+--------------------------------------+
//  | FILE_NAMED_STREAMS                | The file system supports named       |
//  | 0x00040000                        | streams.                             |
//  +-----------------------------------+--------------------------------------+
//  | FILE_SUPPORTS_ENCRYPTION          | The file system supports the         |
//  | 0x00020000                        | Encrypted File System (EFS).<132>    |
//  +-----------------------------------+--------------------------------------+
//  | FILE_SUPPORTS_OBJECT_IDS          | The file system supports object      |
//  | 0x00010000                        | identifiers.                         |
//  +-----------------------------------+--------------------------------------+
//  | FILE_VOLUME_IS_COMPRESSED         | The specified volume is a compressed |
//  | 0x00008000                        | volume. This flag is incompatible    |
//  |                                   | with the FILE_FILE_COMPRESSION flag. |
//  +-----------------------------------+--------------------------------------+
//  | FILE_SUPPORTS_REMOTE_STORAGE      | The file system supports remote      |
//  | 0x00000100                        | storage.<133>                        |
//  +-----------------------------------+--------------------------------------+
//  | FILE_SUPPORTS_REPARSE_POINTS      | The file system supports reparse     |
//  | 0x00000080                        | points.                              |
//  +-----------------------------------+--------------------------------------+
//  | FILE_SUPPORTS_SPARSE_FILES        | The file system supports sparse      |
//  | 0x00000040                        | files.                               |
//  +-----------------------------------+--------------------------------------+
//  | FILE_VOLUME_QUOTAS                | The file system supports per-user    |
//  | 0x00000020                        | quotas.                              |
//  +-----------------------------------+--------------------------------------+
//  | FILE_FILE_COMPRESSION             | The file volume supports file-based  |
//  | 0x00000010                        | compression. This flag is            |
//  |                                   | incompatible with the                |
//  |                                   | FILE_VOLUME_IS_COMPRESSED flag.      |
//  +-----------------------------------+--------------------------------------+
//  | FILE_PERSISTENT_ACLS              | The file system preserves and        |
//  | 0x00000008                        | enforces access control lists        |
//  |                                   | (ACLs).                              |
//  +-----------------------------------+--------------------------------------+
//  | FILE_UNICODE_ON_DISK              | The file system supports Unicode in  |
//  | 0x00000004                        | file and directory names. This flag  |
//  |                                   | applies only to file and directory   |
//  |                                   | names; the file system neither       |
//  |                                   | restricts nor interprets the bytes   |
//  |                                   | of data within a file.               |
//  +-----------------------------------+--------------------------------------+
//  | FILE_CASE_PRESERVED_NAMES         | The file system preserves the case   |
//  | 0x00000002                        | of file names when it places a name  |
//  |                                   | on disk.                             |
//  +-----------------------------------+--------------------------------------+
//  | FILE_CASE_SENSITIVE_SEARCH        | The file system supports             |
//  | 0x00000001                        | case-sensitive file names when       |
//  |                                   | looking up (searching for) file      |
//  |                                   | names in a directory.                |
//  +-----------------------------------+--------------------------------------+
//  | FILE_SUPPORT_INTEGRITY_STREAMS    | The file system supports integrity   |
//  | 0x04000000                        | streams.<134>                        |
//  +-----------------------------------+--------------------------------------+

enum {
      FILE_SUPPORTS_USN_JOURNAL         = 0x02000000
    , FILE_SUPPORTS_OPEN_BY_FILE_ID     = 0x01000000
    , FILE_SUPPORTS_EXTENDED_ATTRIBUTES = 0x00800000
    , FILE_SUPPORTS_HARD_LINKS          = 0x00400000
    , FILE_SUPPORTS_TRANSACTIONS        = 0x00200000
    , FILE_SEQUENTIAL_WRITE_ONCE        = 0x00100000
    , FILE_READ_ONLY_VOLUME             = 0x00080000
    , FILE_NAMED_STREAMS                = 0x00040000
    , FILE_SUPPORTS_ENCRYPTION          = 0x00020000
    , FILE_SUPPORTS_OBJECT_IDS          = 0x00010000
    , FILE_VOLUME_IS_COMPRESSED         = 0x00008000
    , FILE_SUPPORTS_REMOTE_STORAGE      = 0x00000100
    , FILE_SUPPORTS_REPARSE_POINTS      = 0x00000080
    , FILE_SUPPORTS_SPARSE_FILES        = 0x00000040
    , FILE_VOLUME_QUOTAS                = 0x00000020
    , FILE_FILE_COMPRESSION             = 0x00000010
    , FILE_PERSISTENT_ACLS              = 0x00000008
    , FILE_UNICODE_ON_DISK              = 0x00000004
    , FILE_CASE_PRESERVED_NAMES         = 0x00000002
    , FILE_CASE_SENSITIVE_SEARCH        = 0x00000001
    , FILE_SUPPORT_INTEGRITY_STREAMS    = 0x04000000
};

// MaximumComponentNameLength (4 bytes): A 32-bit signed integer that
//  contains the maximum file name component length, in bytes, supported by
//  the specified file system. The value of this field MUST be greater than
//  zero and MUST be no more than 510.<135>

// FileSystemNameLength (4 bytes): A 32-bit unsigned integer that contains
//  the length, in bytes, of the file system name in the FileSystemName
//  field. The value of this field MUST be greater than 0.

// FileSystemName (variable): A variable-length Unicode field containing the
//  name of the file system. This field is not null-terminated and MUST be
//  handled as a sequence of FileSystemNameLength bytes. This field is
//  intended to be informative only. A client SHOULD NOT infer file system
//  type specific behavior from this field.<136>

// This operation returns a status code, as specified in [MS-ERREF] section
//  2.3. The status code returned directly by the function that processes
//  this file information class MUST be STATUS_SUCCESS or one of the
//  following.

//  +-----------------------------+--------------------------------------------+
//  | Error code                  | Meaning                                    |
//  +-----------------------------+--------------------------------------------+
//  | STATUS_INFO_LENGTH_MISMATCH | The specified information record length    |
//  | 0xC0000004                  | does not match the length that is required |
//  |                             | for the specified information class.       |
//  +-----------------------------+--------------------------------------------+
//  | STATUS_BUFFER_OVERFLOW      | The output buffer was filled before all of |
//  | 0x80000005                  | the file system information could be       |
//  |                             | returned; only a portion of the            |
//  |                             | FileSystemName field is returned.          |
//  +-----------------------------+--------------------------------------------+

class FileFsAttributeInformation {
    uint32_t FileSystemAttributes_;
    uint32_t MaximumComponentNameLength;

    std::string file_system_name;

public:
    FileFsAttributeInformation() = default;

    FileFsAttributeInformation(uint32_t FileSystemAttributes,
                               uint32_t MaximumComponentNameLength,
                               const char * file_system_name)
    : FileSystemAttributes_(FileSystemAttributes)
    , MaximumComponentNameLength(MaximumComponentNameLength)
    , file_system_name(file_system_name) {}

    inline void emit(Stream & stream) const {
        stream.out_uint32_le(this->FileSystemAttributes_);
        stream.out_sint32_le(this->MaximumComponentNameLength);

        const size_t maximum_length_of_FileSystemName_in_bytes = this->file_system_name.length() * 2;

        uint8_t * const unicode_data = static_cast<uint8_t *>(::alloca(
                    maximum_length_of_FileSystemName_in_bytes));
        const size_t size_of_unicode_data = ::UTF8toUTF16(
            reinterpret_cast<const uint8_t *>(this->file_system_name.c_str()), unicode_data,
            maximum_length_of_FileSystemName_in_bytes);

        stream.out_uint32_le(size_of_unicode_data); // FileSystemNameLength(4)

        stream.out_copy_bytes(unicode_data, size_of_unicode_data);
    }

    inline void receive(Stream & stream) {
        {
            const unsigned expected = 12;   // FileSystemAttributes(4) + MaximumComponentNameLength(4) +
                                            //     FileSystemNameLength(4)
            if (!stream.in_check_rem(expected)) {
                LOG(LOG_ERR,
                    "Truncated FileFsAttributeInformation (0): expected=%u remains=%u",
                    expected, stream.in_remain());
                throw Error(ERR_FSCC_DATA_TRUNCATED);
            }
        }

        this->FileSystemAttributes_      = stream.in_uint32_le();
        this->MaximumComponentNameLength = stream.in_sint32_le();

        const uint32_t FileSystemNameLength = stream.in_uint32_le();

        {
            const unsigned expected = FileSystemNameLength; // FileSystemName(variable)

            if (!stream.in_check_rem(expected)) {
                LOG(LOG_ERR,
                    "Truncated FileFsAttributeInformation (1): expected=%u remains=%u",
                    expected, stream.in_remain());
                throw Error(ERR_RDPDR_PDU_TRUNCATED);
            }
        }

        uint8_t * const unicode_data = static_cast<uint8_t *>(::alloca(FileSystemNameLength));

        stream.in_copy_bytes(unicode_data, FileSystemNameLength);

        const size_t maximum_length_of_utf8_character_in_bytes = 4;

        const size_t size_of_utf8_string =
            FileSystemNameLength / 2 * maximum_length_of_utf8_character_in_bytes;
        uint8_t * const utf8_string = static_cast<uint8_t *>(
            ::alloca(size_of_utf8_string));
        const size_t length_of_utf8_string = ::UTF16toUTF8(
            unicode_data, FileSystemNameLength / 2, utf8_string, size_of_utf8_string);
        this->file_system_name.assign(::char_ptr_cast(utf8_string),
            length_of_utf8_string);
    }

    inline size_t size() const {
        const size_t size = 12; // FileSystemAttributes(4) + MaximumComponentNameLength(4) +
                                //     FileSystemNameLength(4)

        const size_t maximum_length_of_FileSystemName_in_bytes =
            this->file_system_name.length() * 2;

        uint8_t * const unicode_data = static_cast<uint8_t *>(::alloca(
                    maximum_length_of_FileSystemName_in_bytes));
        const size_t size_of_unicode_data = ::UTF8toUTF16(
            reinterpret_cast<const uint8_t *>(this->file_system_name.c_str()),
            unicode_data, maximum_length_of_FileSystemName_in_bytes);

        return size + size_of_unicode_data;
    }

    uint32_t FileSystemAttributes() const { return this->FileSystemAttributes_; }

    inline void set_FileSystemAttributes(uint32_t FileSystemAttributes) {
        this->FileSystemAttributes_ = FileSystemAttributes;
    }

private:
    size_t str(char * buffer, size_t size) const {
        size_t length = ::snprintf(buffer, size,
            "FileFsAttributeInformation: FileSystemAttributes=0x%X "
                " MaximumComponentNameLength=%u FileSystemName=\"%s\"",
            this->FileSystemAttributes_, this->MaximumComponentNameLength,
            this->file_system_name.c_str());
        return ((length < size) ? length : size - 1);
    }

public:
    inline void log(int level) const {
        char buffer[2048];
        this->str(buffer, sizeof(buffer));
        buffer[sizeof(buffer) - 1] = 0;
        LOG(level, buffer);
    }
};  // FileFsAttributeInformation

// [MS-FSCC] - 2.5.8 FileFsSizeInformation
// =======================================

// This information class is used to query sector size information for a file
//  system volume.

// A FILE_FS_SIZE_INFORMATION data element, defined as follows, is returned
//  by the server.

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | | | | | | | | | | |1| | | | | | | | | |2| | | | | | | | | |3| |
// |0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                      TotalAllocationUnits                     |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                    AvailableAllocationUnits                   |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                    SectorsPerAllocationUnit                   |
// +---------------------------------------------------------------+
// |                         BytesPerSector                        |
// +---------------------------------------------------------------+

// TotalAllocationUnits (8 bytes): A 64-bit signed integer that contains the
//  total number of allocation units on the volume that are available to the
//  user associated with the calling thread. This value MUST be greater than
//  or equal to 0.<152>

// AvailableAllocationUnits (8 bytes): A 64-bit signed integer that contains
//  the total number of free allocation units on the volume that are
//  available to the user associated with the calling thread. This value MUST
//  be greater than or equal to 0.<153>

// SectorsPerAllocationUnit (4 bytes): A 32-bit unsigned integer that
//  contains the number of sectors in each allocation unit.

// BytesPerSector (4 bytes): A 32-bit unsigned integer that contains the
//  number of bytes in each sector.

// This operation returns a status code, as specified in [MS-ERREF] section
//  2.3. The status code returned directly by the function that processes
//  this file information class MUST be STATUS_SUCCESS or one of the
//  following.

//  +-----------------------------+--------------------------------------------+
//  | Error code                  | Meaning                                    |
//  +-----------------------------+--------------------------------------------+
//  | STATUS_INFO_LENGTH_MISMATCH | The specified information record length    |
//  | 0xC0000004                  | does not match the length that is required |
//  |                             | for the specified information class.       |
//  +-----------------------------+--------------------------------------------+

class FileFsSizeInformation {
    int64_t  TotalAllocationUnits     = 0;
    int64_t  AvailableAllocationUnits = 0;
    uint32_t SectorsPerAllocationUnit = 0;
    uint32_t BytesPerSector           = 0;

public:
    FileFsSizeInformation() = default;

    FileFsSizeInformation(int64_t TotalAllocationUnits,
                          int64_t AvailableAllocationUnits,
                          uint32_t SectorsPerAllocationUnit,
                          uint32_t BytesPerSector)
    : TotalAllocationUnits(TotalAllocationUnits)
    , AvailableAllocationUnits(AvailableAllocationUnits)
    , SectorsPerAllocationUnit(SectorsPerAllocationUnit)
    , BytesPerSector(BytesPerSector) {}

    inline void emit(Stream & stream) const {
        stream.out_sint64_le(this->TotalAllocationUnits);
        stream.out_sint64_le(this->AvailableAllocationUnits);
        stream.out_uint32_le(this->SectorsPerAllocationUnit);
        stream.out_uint32_le(this->BytesPerSector);
    }

    inline void receive(Stream & stream) {
        {
            const unsigned expected = 24;   // TotalAllocationUnits(8) +
                                            //     AvailableAllocationUnits(8) +
                                            //     SectorsPerAllocationUnit(4) +
                                            //     BytesPerSector(4)
            if (!stream.in_check_rem(expected)) {
                LOG(LOG_ERR,
                    "Truncated FileFsSizeInformation: expected=%u remains=%u",
                    expected, stream.in_remain());
                throw Error(ERR_FSCC_DATA_TRUNCATED);
            }
        }

        this->TotalAllocationUnits     = stream.in_sint64_le();
        this->AvailableAllocationUnits = stream.in_sint64_le();
        this->SectorsPerAllocationUnit = stream.in_uint32_le();
        this->BytesPerSector           = stream.in_uint32_le();
    }

    inline size_t size() const {
        return 24;  // TotalAllocationUnits(8) + AvailableAllocationUnits(8) +
                    //     SectorsPerAllocationUnit(4) + BytesPerSector(4)
    }

private:
    size_t str(char * buffer, size_t size) const {
        size_t length = ::snprintf(buffer, size,
            "FileFsSizeInformation: TotalAllocationUnits=%" PRId64
                " AvailableAllocationUnits=%" PRId64
                " SectorsPerAllocationUnit=%u BytesPerSector=%u",
            this->TotalAllocationUnits, this->AvailableAllocationUnits,
            this->SectorsPerAllocationUnit, this->BytesPerSector);
        return ((length < size) ? length : size - 1);
    }

public:
    inline void log(int level) const {
        char buffer[2048];
        this->str(buffer, sizeof(buffer));
        buffer[sizeof(buffer) - 1] = 0;
        LOG(level, buffer);
    }
};

// [MS-FSCC] - 2.5.9 FileFsVolumeInformation
// =========================================

// This information class is used to query information on a volume on which a
//  file system is mounted. A FILE_FS_VOLUME_INFORMATION data element,
//  defined as follows, is returned by the server.

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | | | | | | | | | | |1| | | | | | | | | |2| | | | | | | | | |3| |
// |0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                       VolumeCreationTime                      |
// +---------------------------------------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+
// |                       VolumeSerialNumber                      |
// +---------------------------------------------------------------+
// |                       VolumeLabelLength                       |
// +---------------+---------------+-------------------------------+
// |SupportsObjects|    Reserved   |     VolumeLabel (variable)    |
// +---------------+---------------+-------------------------------+
// |                              ...                              |
// +---------------------------------------------------------------+

// VolumeCreationTime (8 bytes): The time when the volume was created; see
//  section 2.1.1. The value of this field MUST be greater than or equal to
//  0.

// VolumeSerialNumber (4 bytes): A 32-bit unsigned integer that contains the
//  serial number of the volume. The serial number is an opaque value
//  generated by the file system at format time, and is not necessarily
//  related to any hardware serial number for the device on which the file
//  system is located. No specific format or content of this field is
//  required for protocol interoperation. This value is not required to be
//  unique.

// VolumeLabelLength (4 bytes): A 32-bit unsigned integer that contains the
//  length, in bytes, including the trailing null, if present, of the name of
//  the volume.<154>

// SupportsObjects (1 byte): A Boolean (section 2.1.8) value. Set to TRUE if
//  the file system supports object-oriented file system objects; set to
//  FALSE otherwise.<155>

// Reserved (1 byte): An 8-bit field. This field is reserved. This field MUST
//  be set to zero and MUST be ignored.

// VolumeLabel (variable): A variable-length Unicode field containing the
//  name of the volume. The content of this field can be a null-terminated
//  string or can be a string padded with the space character to be
//  VolumeLabelLength bytes long.

// This operation returns a status code, as specified in [MS-ERREF] section
//  2.3. The status code returned directly by the function that processes
//  this file information class MUST be STATUS_SUCCESS or one of the
//  following.

//  +-----------------------------+--------------------------------------------+
//  | Error code                  | Meaning                                    |
//  +-----------------------------+--------------------------------------------+
//  | STATUS_INFO_LENGTH_MISMATCH | The specified information record length    |
//  | 0xC0000004                  | does not match the length that is required |
//  |                             | for the specified information class.       |
//  +-----------------------------+--------------------------------------------+
//  | STATUS_BUFFER_OVERFLOW      | The output buffer was filled before all of |
//  | 0x80000005                  | the volume information could be returned;  |
//  |                             | only a portion of the VolumeLabel field is |
//  |                             | returned.                                  |
//  +-----------------------------+--------------------------------------------+

class FileFsVolumeInformation {
    uint64_t VolumeCreationTime = 0;
    uint32_t VolumeSerialNumber = 0;
    uint8_t  SupportsObjects    = 0;

    std::string volume_label;

public:
    FileFsVolumeInformation() = default;

    FileFsVolumeInformation(uint64_t VolumeCreationTime, uint32_t VolumeSerialNumber,
                         uint8_t SupportsObjects, const char * volume_label)
    : VolumeCreationTime(VolumeCreationTime)
    , VolumeSerialNumber(VolumeSerialNumber)
    , SupportsObjects(SupportsObjects)
    , volume_label(volume_label) {}

    inline void emit(Stream & stream) const {
        stream.out_uint64_le(this->VolumeCreationTime);
        stream.out_uint32_le(this->VolumeSerialNumber);

        // The null-terminator is included.
        const size_t maximum_length_of_VolumeLabel_in_bytes = (this->volume_label.length() + 1) * 2;

        uint8_t * const unicode_data = static_cast<uint8_t *>(::alloca(
                    maximum_length_of_VolumeLabel_in_bytes));
        size_t size_of_unicode_data = ::UTF8toUTF16(
            reinterpret_cast<const uint8_t *>(this->volume_label.c_str()), unicode_data,
            maximum_length_of_VolumeLabel_in_bytes);
        // Writes null terminator.
        unicode_data[size_of_unicode_data    ] =
        unicode_data[size_of_unicode_data + 1] = 0;
        size_of_unicode_data += 2;

        stream.out_uint32_le(size_of_unicode_data); // VolumeLabelLength(4)

        stream.out_uint8(this->SupportsObjects);

        // Reserved(1), MUST NOT be transmitted.

        stream.out_copy_bytes(unicode_data, size_of_unicode_data);
    }

    inline void receive(Stream & stream) {
        {
            const unsigned expected = 17;   // VolumeCreationTime(8) + VolumeSerialNumber(4) +
                                            //     VolumeLabelLength(4) + SupportsObjects(1)
            if (!stream.in_check_rem(expected)) {
                LOG(LOG_ERR,
                    "Truncated FileFsVolumeInformation (0): expected=%u remains=%u",
                    expected, stream.in_remain());
                throw Error(ERR_FSCC_DATA_TRUNCATED);
            }
        }

        this->VolumeCreationTime = stream.in_uint64_le();
        this->VolumeSerialNumber = stream.in_uint32_le();

        const uint32_t VolumeLabelLength = stream.in_uint32_le();

        this->SupportsObjects  = stream.in_uint8();

        // Reserved(1), MUST NOT be transmitted.

        {
            const unsigned expected = VolumeLabelLength;    // VolumeLabel(variable)

            if (!stream.in_check_rem(expected)) {
                LOG(LOG_ERR,
                    "Truncated FileFsVolumeInformation (1): expected=%u remains=%u",
                    expected, stream.in_remain());
                throw Error(ERR_RDPDR_PDU_TRUNCATED);
            }
        }

        uint8_t * const unicode_data = static_cast<uint8_t *>(::alloca(VolumeLabelLength));

        stream.in_copy_bytes(unicode_data, VolumeLabelLength);

        const size_t maximum_length_of_utf8_character_in_bytes = 4;

        const size_t size_of_utf8_string =
            VolumeLabelLength / 2 * maximum_length_of_utf8_character_in_bytes + 1;
        uint8_t * const utf8_string = static_cast<uint8_t *>(
            ::alloca(size_of_utf8_string));
        const size_t length_of_utf8_string = ::UTF16toUTF8(
            unicode_data, VolumeLabelLength / 2, utf8_string, size_of_utf8_string);

        for (uint8_t * c = utf8_string + length_of_utf8_string - 1;
             (c >= utf8_string) && ((*c) == ' '); c--) {
            *c = '\0';
        }

        this->volume_label = ::char_ptr_cast(utf8_string);
    }

    inline size_t size() const {
        const size_t size = 17; // VolumeCreationTime(8) + VolumeSerialNumber(4) +
                                //     VolumeLabelLength(4) + SupportsObjects(1)

        // Reserved(1), MUST NOT be transmitted.

        // The null-terminator is included.
        const size_t maximum_length_of_VolumeLabel_in_bytes = (this->volume_label.length() + 1) * 2;

        uint8_t * const unicode_data = static_cast<uint8_t *>(::alloca(
                    maximum_length_of_VolumeLabel_in_bytes));
        size_t size_of_unicode_data = ::UTF8toUTF16(
            reinterpret_cast<const uint8_t *>(this->volume_label.c_str()), unicode_data,
            maximum_length_of_VolumeLabel_in_bytes);
        // Writes null terminator.
        unicode_data[size_of_unicode_data    ] =
        unicode_data[size_of_unicode_data + 1] = 0;
        size_of_unicode_data += 2;

        return size + size_of_unicode_data;
    }

private:
    size_t str(char * buffer, size_t size) const {
        size_t length = ::snprintf(buffer, size,
            "FileFsVolumeInformation: VolumeCreationTime=%" PRIu64
                " VolumeSerialNumber=0x%X SupportsObjects=%u VolumeLabel=\"%s\"",
            this->VolumeCreationTime, this->VolumeSerialNumber,
            this->SupportsObjects, this->volume_label.c_str());
        return ((length < size) ? length : size - 1);
    }

public:
    inline void log(int level) const {
        char buffer[2048];
        this->str(buffer, sizeof(buffer));
        buffer[sizeof(buffer) - 1] = 0;
        LOG(level, buffer);
    }
};  // FileFsVolumeInformation

// [MS-FSCC] - 2.6 File Attributes
// ===============================

// The following attributes are defined for files and directories. They can
//  be used in any combination unless noted in the description of the
//  attribute's meaning. There is no file attribute with the value 0x00000000
//  because a value of 0x00000000 in the FileAttributes field means that the
//  file attributes for this file MUST NOT be changed when setting basic
//  information for the file.

//  +------------------------------------+-------------------------------------+
//  | Value                              | Meaning                             |
//  +------------------------------------+-------------------------------------+
//  | FILE_ATTRIBUTE_ARCHIVE             | A file or directory that requires   |
//  | 0x00000020                         | to be archived. Applications use    |
//  |                                    | this attribute to mark files for    |
//  |                                    | backup or removal.                  |
//  +------------------------------------+-------------------------------------+
//  | FILE_ATTRIBUTE_COMPRESSED          | A file or directory that is         |
//  | 0x00000800                         | compressed. For a file, all of the  |
//  |                                    | data in the file is compressed. For |
//  |                                    | a directory, compression is the     |
//  |                                    | default for newly created files and |
//  |                                    | subdirectories.                     |
//  +------------------------------------+-------------------------------------+
//  | FILE_ATTRIBUTE_DIRECTORY           | This item is a directory.           |
//  | 0x00000010                         |                                     |
//  +------------------------------------+-------------------------------------+
//  | FILE_ATTRIBUTE_ENCRYPTED           | A file or directory that is         |
//  | 0x00004000                         | encrypted. For a file, all data     |
//  |                                    | streams in the file are encrypted.  |
//  |                                    | For a directory, encryption is the  |
//  |                                    | default for newly created files and |
//  |                                    | subdirectories.                     |
//  +------------------------------------+-------------------------------------+
//  | FILE_ATTRIBUTE_HIDDEN              | A file or directory that is hidden. |
//  | 0x00000002                         | Files and directories marked with   |
//  |                                    | this attribute do not appear in an  |
//  |                                    | ordinary directory listing.         |
//  +------------------------------------+-------------------------------------+
//  | FILE_ATTRIBUTE_NORMAL              | A file that does not have other     |
//  | 0x00000080                         | attributes set. This flag is used   |
//  |                                    | to clear all other flags by         |
//  |                                    | specifying it with no other flags   |
//  |                                    | set.                                |
//  |                                    | This flag MUST be ignored if other  |
//  |                                    | flags are set.<157>                 |
//  +------------------------------------+-------------------------------------+
//  | FILE_ATTRIBUTE_NOT_CONTENT_INDEXED | A file or directory that is not     |
//  | 0x00002000                         | indexed by the content indexing     |
//  |                                    | service.                            |
//  +------------------------------------+-------------------------------------+
//  | FILE_ATTRIBUTE_OFFLINE             | The data in this file is not        |
//  | 0x00001000                         | available immediately. This         |
//  |                                    | attribute indicates that the file   |
//  |                                    | data is physically moved to offline |
//  |                                    | storage. This attribute is used by  |
//  |                                    | Remote Storage, which is            |
//  |                                    | hierarchical storage management     |
//  |                                    | software.                           |
//  +------------------------------------+-------------------------------------+
//  | FILE_ATTRIBUTE_READONLY            | A file or directory that is read-   |
//  | 0x00000001                         |only. For a file, applications can   |
//  |                                    | read the file but cannot write to   |
//  |                                    | it or delete it. For a directory,   |
//  |                                    | applications cannot delete it, but  |
//  |                                    | applications can create and delete  |
//  |                                    | files from that directory.          |
//  +------------------------------------+-------------------------------------+
//  | FILE_ATTRIBUTE_REPARSE_POINT       | A file or directory that has an     |
//  | 0x00000400                         | associated reparse point.           |
//  +------------------------------------+-------------------------------------+
//  | FILE_ATTRIBUTE_SPARSE_FILE         | A file that is a sparse file.       |
//  | 0x00000200                         |                                     |
//  +------------------------------------+-------------------------------------+
//  | FILE_ATTRIBUTE_SYSTEM              | A file or directory that the        |
//  | 0x00000004                         | operating system uses a part of or  |
//  |                                    | uses exclusively.                   |
//  +------------------------------------+-------------------------------------+
//  | FILE_ATTRIBUTE_TEMPORARY           | A file that is being used for       |
//  | 0x00000100                         | temporary storage. The operating    |
//  |                                    | system may choose to store this     |
//  |                                    | file's data in memory rather than   |
//  |                                    | on mass storage, writing the data   |
//  |                                    | to mass storage only if data        |
//  |                                    | remains in the file when the file   |
//  |                                    | is closed.                          |
//  +------------------------------------+-------------------------------------+
//  | FILE_ATTRIBUTE_INTEGRITY_STREAM    | A file or directory that is         |
//  | 0x00008000                         | configured with integrity support.  |
//  |                                    | For a file, all data streams in the |
//  |                                    | file have integrity support. For a  |
//  |                                    | directory, integrity support is the |
//  |                                    | default for newly created files and |
//  |                                    | subdirectories, unless the caller   |
//  |                                    | specifies otherwise.<158>           |
//  +------------------------------------+-------------------------------------+
//  | FILE_ATTRIBUTE_NO_SCRUB_DATA       | A file or directory that is         |
//  | 0x00020000                         | configured to be excluded from the  |
//  |                                    | data integrity scan. For a          |
//  |                                    | directory configured with           |
//  |                                    | FILE_ATTRIBUTE_NO_SCRUB_DATA, the   |
//  |                                    | default for newly created files and |
//  |                                    | subdirectories is to inherit the    |
//  |                                    | FILE_ATTRIBUTE_NO_SCRUB_DATA        |
//  |                                    | attribute.<159>                     |
//  +------------------------------------+-------------------------------------+

enum {
      FILE_ATTRIBUTE_ARCHIVE             = 0x00000020
    , FILE_ATTRIBUTE_COMPRESSED          = 0x00000800
    , FILE_ATTRIBUTE_DIRECTORY           = 0x00000010
    , FILE_ATTRIBUTE_ENCRYPTED           = 0x00004000
    , FILE_ATTRIBUTE_HIDDEN              = 0x00000002
    , FILE_ATTRIBUTE_NORMAL              = 0x00000080
    , FILE_ATTRIBUTE_NOT_CONTENT_INDEXED = 0x00002000
    , FILE_ATTRIBUTE_OFFLINE             = 0x00001000
    , FILE_ATTRIBUTE_READONLY            = 0x00000001
    , FILE_ATTRIBUTE_REPARSE_POINT       = 0x00000400
    , FILE_ATTRIBUTE_SPARSE_FILE         = 0x00000200
    , FILE_ATTRIBUTE_SYSTEM              = 0x00000004
    , FILE_ATTRIBUTE_TEMPORARY           = 0x00000100
    , FILE_ATTRIBUTE_INTEGRITY_STREAM    = 0x00008000
    , FILE_ATTRIBUTE_NO_SCRUB_DATA       = 0x00020000
};

}   // namespace fscc

#endif  // #ifndef _REDEMPTION_CORE_FSCC_FILEINFORMATION_HPP_
