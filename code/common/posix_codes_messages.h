#pragma once
#ifndef POSIX_INC_
#define POSIX_INC_

#include "config.h"

/*
(c) 2020 by dbj@dbj.org CC BY SA 4.0

here we define POSIX constants so we do not depend on std lib
*/

#ifdef __cplusplus
namespace dbj {
	extern "C" {
#endif // __cplusplus


		// tags for POSIX error codes
		// matching errno constants are in capitals in comments
		typedef enum dbj_errc {
			address_family_not_supported = 102, // EAFNOSUPPORT
			address_in_use = 100, // EADDRINUSE
			address_not_available = 101, // EADDRNOTAVAIL
			already_connected = 113, // EISCONN
			argument_list_too_long = 7, // E2BIG
			argument_out_of_domain = 33, // EDOM
			bad_address = 14, // EFAULT
			bad_file_descriptor = 9, // EBADF
			bad_message = 104, // EBADMSG
			broken_pipe = 32, // EPIPE
			connection_aborted = 106, // ECONNABORTED
			connection_already_in_progress = 103, // EALREADY
			connection_refused = 107, // ECONNREFUSED
			connection_reset = 108, // ECONNRESET
			cross_device_link = 18, // EXDEV
			destination_address_required = 109, // EDESTADDRREQ
			device_or_resource_busy = 16, // EBUSY
			directory_not_empty = 41, // ENOTEMPTY
			executable_format_error = 8, // ENOEXEC
			file_exists = 17, // EEXIST
			file_too_large = 27, // EFBIG
			filename_too_long = 38, // ENAMETOOLONG
			function_not_supported = 40, // ENOSYS
			host_unreachable = 110, // EHOSTUNREACH
			identifier_removed = 111, // EIDRM
			illegal_byte_sequence = 42, // EILSEQ
			inappropriate_io_control_operation = 25, // ENOTTY
			interrupted = 4, // EINTR
			invalid_argument = 22, // EINVAL
			invalid_seek = 29, // ESPIPE
			io_error = 5, // EIO
			is_a_directory = 21, // EISDIR
			message_size = 115, // EMSGSIZE
			network_down = 116, // ENETDOWN
			network_reset = 117, // ENETRESET
			network_unreachable = 118, // ENETUNREACH
			no_buffer_space = 119, // ENOBUFS
			no_child_process = 10, // ECHILD
			no_link = 121, // ENOLINK
			no_lock_available = 39, // ENOLCK
			no_message_available = 120, // ENODATA
			no_message = 122, // ENOMSG
			no_protocol_option = 123, // ENOPROTOOPT
			no_space_on_device = 28, // ENOSPC
			no_stream_resources = 124, // ENOSR
			no_such_device_or_address = 6, // ENXIO
			no_such_device = 19, // ENODEV
			no_such_file_or_directory = 2, // ENOENT
			no_such_process = 3, // ESRCH
			not_a_directory = 20, // ENOTDIR
			not_a_socket = 128, // ENOTSOCK
			not_a_stream = 125, // ENOSTR
			not_connected = 126, // ENOTCONN
			not_enough_memory = 12, // ENOMEM
			not_supported = 129, // ENOTSUP
			operation_canceled = 105, // ECANCELED
			operation_in_progress = 112, // EINPROGRESS
			operation_not_permitted = 1, // EPERM
			operation_not_supported = 130, // EOPNOTSUPP
			operation_would_block = 140, // EWOULDBLOCK
			owner_dead = 133, // EOWNERDEAD
			permission_denied = 13, // EACCES
			protocol_error = 134, // EPROTO
			protocol_not_supported = 135, // EPROTONOSUPPORT
			read_only_file_system = 30, // EROFS
			resource_deadlock_would_occur = 36, // EDEADLK
			resource_unavailable_try_again = 11, // EAGAIN
			result_out_of_range = 34, // ERANGE
			state_not_recoverable = 127, // ENOTRECOVERABLE
			stream_timeout = 137, // ETIME
			text_file_busy = 139, // ETXTBSY
			timed_out = 138, // ETIMEDOUT
			too_many_files_open_in_system = 23, // ENFILE
			too_many_files_open = 24, // EMFILE
			too_many_links = 31, // EMLINK
			too_many_symbolic_link_levels = 114, // ELOOP
			value_too_large = 132, // EOVERFLOW
			wrong_protocol_type = 136 // EPROTOTYPE
		}
		dbj_errc ;

		DBJ_PURE_FUN
			static inline
			const char* const dbj_errc_message(dbj_errc code_)
			noexcept
		{
			switch (code_)
			{
			case   address_family_not_supported:  		return "Address family not supported";
			case   address_in_use:  					return "Address in use";
			case   address_not_available:  				return "Address not available";
			case   already_connected:  					return "Already connected";
			case   argument_list_too_long:  			return "Argument list too long";
			case   argument_out_of_domain:  			return "Argument out of domain";
			case   bad_address:  						return "Bad address";
			case   bad_file_descriptor:  				return "Bad file descriptor";
			case   bad_message:  						return "Bad message";
			case   broken_pipe:  						return "Broken pipe";
			case   connection_aborted:  				return "Connection aborted";
			case   connection_already_in_progress:  	return "Connection already in progress";
			case   connection_refused:  				return "Connection refused";
			case   connection_reset:  					return "Connection reset";
			case   cross_device_link:  					return "Cross device link";
			case   destination_address_required:  		return "Destination address required";
			case   device_or_resource_busy:  			return "Device or resource busy";
			case   directory_not_empty:  				return "Directory not empty";
			case   executable_format_error:  			return "Executable format error";
			case   file_exists:  						return "File exists";
			case   file_too_large:  					return "File too large";
			case   filename_too_long:  					return "Filename too long";
			case   function_not_supported:  			return "Function not supported";
			case   host_unreachable:  					return "Host unreachable";
			case   identifier_removed:  				return "Identifier removed";
			case   illegal_byte_sequence:  				return "Illegal byte sequence";
			case   inappropriate_io_control_operation:  return "Inappropriate io control operation";
			case   interrupted:  						return "Interrupted";
			case   invalid_argument:  					return "Invalid argument";
			case   invalid_seek:  						return "Invalid seek";
			case   io_error:  						    return "Io error";
			case   is_a_directory:  					return "Is a directory";
			case   message_size:  						return "Message size";
			case   network_down:  						return "Network down";
			case   network_reset:  						return "Network reset";
			case   network_unreachable:  				return "Network unreachable";
			case   no_buffer_space:  					return "No buffer space";
			case   no_child_process:  					return "No child process";
			case   no_link:  						    return "No link";
			case   no_lock_available:  					return "No lock available";
			case   no_message_available:  				return "No message available";
			case   no_message:  						return "No message";
			case   no_protocol_option:  				return "No protocol option";
			case   no_space_on_device:  				return "No space on device";
			case   no_stream_resources:  				return "No stream resources";
			case   no_such_device_or_address:  			return "No such device or address";
			case   no_such_device:  					return "No such device";
			case   no_such_file_or_directory:  			return "No such file or directory";
			case   no_such_process:  					return "No such process";
			case   not_a_directory:  					return "Not a directory";
			case   not_a_socket:  						return "Not a socket";
			case   not_a_stream:  						return "Not a stream";
			case   not_connected:  						return "Not connected";
			case   not_enough_memory:  					return "Not enough memory";
			case   not_supported:  						return "Not supported";
			case   operation_canceled:  				return "Operation canceled";
			case   operation_in_progress:  				return "Operation in progress";
			case   operation_not_permitted:  			return "Operation not permitted";
			case   operation_not_supported:  			return "Operation not supported";
			case   operation_would_block:  				return "Operation would block";
			case   owner_dead:  						return "Owner dead";
			case   permission_denied:  					return "Permission denied";
			case   protocol_error:  					return "Protocol error";
			case   protocol_not_supported:  			return "Protocol not supported";
			case   read_only_file_system:  				return "Read only file system";
			case   resource_deadlock_would_occur:  		return "Resource deadlock would occur";
			case   resource_unavailable_try_again:  	return "Resource unavailable try again";
			case   result_out_of_range:  				return "Result out of range";
			case   state_not_recoverable:  				return "State not recoverable";
			case   stream_timeout:  					return "Stream timeout";
			case   text_file_busy:  					return "Text file busy";
			case   timed_out:  						    return "Timed out";
			case   too_many_files_open_in_system:  		return "Too many files open in system";
			case   too_many_files_open:  				return "Too many files open";
			case   too_many_links:  					return "Too many links";
			case   too_many_symbolic_link_levels:  		return "Too many symbolic link levels";
			case   value_too_large:  					return "Value too large";
			case   wrong_protocol_type:  				return "Wrong protocol type";
			default:
				return __FILE__ " Unknown POSIX error code";
			} // switch
		} // error_message()

#ifdef __cplusplus
	} //extern "C" 
} //   namespace dbj 
#endif // __cplusplus

#endif // !POSIX_INC_

