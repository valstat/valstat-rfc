
#ifndef HTTP_STATUS_CODES_INC_
#define HTTP_STATUS_CODES_INC_

/// dbj.org CC BY SA 4.0
/// dbj.org changed the case to all small, introduced stdbool etc

#include "config.h"

#include <assert.h>
#include <stdbool.h>
/*
 *
 * HTTP Status Codes - C Variant
 *
 * https://github.com/j-ulrich/http-status-codes-cpp
 *
 * \version 1.3.0
 * \author Jochen Ulrich <jochenulrich@t-online.de>
 * \copyright Licensed under Creative Commons CC0 (http://creativecommons.org/publicdomain/zero/1.0/)
 */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/*! Enum for the HTTP status codes.
	 */
	typedef enum http_status_code
	{

		/*####### 1xx - Informational #######*/
		/* Indicates an interim response for communicating connection status
		 * or request progress prior to completing the requested action and
		 * sending a final response.
		 */
		http_status_continue = 100, /*!< Indicates that the initial part of a request has been received and has not yet been rejected by the server. */
		http_status_switching_protocols = 101, /*!< Indicates that the server understands and is willing to comply with the client's request, via the Upgrade header field, for a change in the application protocol being used on this connection. */
		http_status_processing = 102, /*!< Is an interim response used to inform the client that the server has accepted the complete request, but has not yet completed it. */
		http_status_eerly_hints = 103, /*!< Indicates to the client that the server is likely to send a final response with the header fields included in the informational response. */

		/*####### 2xx - Successful #######*/
		/* Indicates that the client's request was successfully received,
		 * understood, and accepted.
		 */
		 http_status_ok = 200, /*!< Indicates that the request has succeeded. */
		 http_status_created = 201, /*!< Indicates that the request has been fulfilled and has resulted in one or more new resources being created. */
		 http_status_accepted = 202, /*!< Indicates that the request has been accepted for processing, but the processing has not been completed. */
		 http_status_non_authoritative_information = 203, /*!< Indicates that the request was successful but the enclosed payload has been modified from that of the origin server's 200 (OK) response by a transforming proxy. */
		 http_status_no_content = 204, /*!< Indicates that the server has successfully fulfilled the request and that there is no additional content to send in the response payload body. */
		 http_status_reset_content = 205, /*!< Indicates that the server has fulfilled the request and desires that the user agent reset the \"document view\", which caused the request to be sent, to its original state as received from the origin server. */
		 http_status_partial_content = 206, /*!< Indicates that the server is successfully fulfilling a range request for the target resource by transferring one or more parts of the selected representation that correspond to the satisfiable ranges found in the requests's Range header field. */
		 http_status_multi_status = 207, /*!< Provides status for multiple independent operations. */
		 http_status_already_reported = 208, /*!< Used inside a DAV:propstat response element to avoid enumerating the internal members of multiple bindings to the same collection repeatedly. [RFC 5842] */
		 http_status_imused = 226, /*!< The server has fulfilled a GET request for the resource, and the response is a representation of the result of one or more instance-manipulations applied to the current instance. */

		 /*####### 3xx - Redirection #######*/
		 /* Indicates that further action needs to be taken by the user agent
		  * in order to fulfill the request.
		  */
		  http_status_multiple_choices = 300, /*!< Indicates that the target resource has more than one representation, each with its own more specific identifier, and information about the alternatives is being provided so that the user (or user agent) can select a preferred representation by redirecting its request to one or more of those identifiers. */
		  http_status_moved_permanently = 301, /*!< Indicates that the target resource has been assigned a new permanent URI and any future references to this resource ought to use one of the enclosed URIs. */
		  http_status_found = 302, /*!< Indicates that the target resource resides temporarily under a different URI. */
		  http_status_see_other = 303, /*!< Indicates that the server is redirecting the user agent to a different resource, as indicated by a URI in the Location header field, that is intended to provide an indirect response to the original request. */
		  http_status_not_modified = 304, /*!< Indicates that a conditional GET request has been received and would have resulted in a 200 (OK) response if it were not for the fact that the condition has evaluated to false. */
		  http_status_use_proxy = 305, /*!< \deprecated \parblock Due to security concerns regarding in-band configuration of a proxy. \endparblock
	 The requested resource MUST be accessed through the proxy given by the Location field. */
	 http_status_temporary_redirect = 307, /*!< Indicates that the target resource resides temporarily under a different URI and the user agent MUST NOT change the request method if it performs an automatic redirection to that URI. */
	 http_status_permanent_redirect = 308, /*!< The target resource has been assigned a new permanent URI and any future references to this resource ought to use one of the enclosed URIs. [...] This status code is similar to 301 Moved Permanently (Section 7.3.2 of rfc7231), except that it does not allow rewriting the request method from POST to GET. */

	 /*####### 4xx - Client Error #######*/
	 /* Indicates that the client seems to have erred.
		*/
		http_status_bad_request = 400, /*!< Indicates that the server cannot or will not process the request because the received syntax is invalid, nonsensical, or exceeds some limitation on what the server is willing to process. */
		http_status_unauthorized = 401, /*!< Indicates that the request has not been applied because it lacks valid authentication credentials for the target resource. */
		http_status_payment_required = 402, /*!< *Reserved* */
		http_status_forbidden = 403, /*!< Indicates that the server understood the request but refuses to authorize it. */
		http_status_not_found = 404, /*!< Indicates that the origin server did not find a current representation for the target resource or is not willing to disclose that one exists. */
		http_status_method_not_allowed = 405, /*!< Indicates that the method specified in the request-line is known by the origin server but not supported by the target resource. */
		http_status_not_acceptable = 406, /*!< Indicates that the target resource does not have a current representation that would be acceptable to the user agent, according to the proactive negotiation header fields received in the request, and the server is unwilling to supply a default representation. */
		http_status_proxy_authentication_required = 407, /*!< Is similar to 401 (Unauthorized), but indicates that the client needs to authenticate itself in order to use a proxy. */
		http_status_request_timeout = 408, /*!< Indicates that the server did not receive a complete request message within the time that it was prepared to wait. */
		http_status_conflict = 409, /*!< Indicates that the request could not be completed due to a conflict with the current state of the resource. */
		http_status_gone = 410, /*!< Indicates that access to the target resource is no longer available at the origin server and that this condition is likely to be permanent. */
		http_status_length_required = 411, /*!< Indicates that the server refuses to accept the request without a defined Content-Length. */
		http_status_precondition_failed = 412, /*!< Indicates that one or more preconditions given in the request header fields evaluated to false when tested on the server. */
		http_status_payload_too_large = 413, /*!< Indicates that the server is refusing to process a request because the request payload is larger than the server is willing or able to process. */
		http_status_uri_too_long = 414, /*!< Indicates that the server is refusing to service the request because the request-target is longer than the server is willing to interpret. */
		http_status_unsupported_media_type = 415, /*!< Indicates that the origin server is refusing to service the request because the payload is in a format not supported by the target resource for this method. */
		http_status_range_not_satisfiable = 416, /*!< Indicates that none of the ranges in the request's Range header field overlap the current extent of the selected resource or that the set of ranges requested has been rejected due to invalid ranges or an excessive request of small or overlapping ranges. */
		http_status_expectation_failed = 417, /*!< Indicates that the expectation given in the request's Expect header field could not be met by at least one of the inbound servers. */
		http_status_im_a_teapot = 418, /*!< Any attempt to brew coffee with a teapot should result in the error code 418 I'm a teapot. */
		http_status_unprocessable_entity = 422, /*!< Means the server understands the content type of the request entity (hence a 415(Unsupported Media Type) status code is inappropriate), and the syntax of the request entity is correct (thus a 400 (Bad Request) status code is inappropriate) but was unable to process the contained instructions. */
		http_status_locked = 423, /*!< Means the source or destination resource of a method is locked. */
		http_status_failed_dependency = 424, /*!< Means that the method could not be performed on the resource because the requested action depended on another action and that action failed. */
		http_status_upgrade_required = 426, /*!< Indicates that the server refuses to perform the request using the current protocol but might be willing to do so after the client upgrades to a different protocol. */
		http_status_precondition_required = 428, /*!< Indicates that the origin server requires the request to be conditional. */
		http_status_too_many_requests = 429, /*!< Indicates that the user has sent too many requests in a given amount of time (\"rate limiting\"). */
		http_status_request_header_fields_too_large = 431, /*!< Indicates that the server is unwilling to process the request because its header fields are too large. */
		http_status_unavailable_for_legal_reasons = 451, /*!< This status code indicates that the server is denying access to the resource in response to a legal demand. */

		/*####### 5xx - Server Error #######*/
		/* Indicates that the server is aware that it has erred
		 * or is incapable of performing the requested method.
		 */
		 http_status_internal_server_error = 500, /*!< Indicates that the server encountered an unexpected condition that prevented it from fulfilling the request. */
		 http_status_not_implemented = 501, /*!< Indicates that the server does not support the functionality required to fulfill the request. */
		 http_status_bad_gateway = 502, /*!< Indicates that the server, while acting as a gateway or proxy, received an invalid response from an inbound server it accessed while attempting to fulfill the request. */
		 http_status_service_unavailable = 503, /*!< Indicates that the server is currently unable to handle the request due to a temporary overload or scheduled maintenance, which will likely be alleviated after some delay. */
		 http_status_gateway_timeout = 504, /*!< Indicates that the server, while acting as a gateway or proxy, did not receive a timely response from an upstream server it needed to access in order to complete the request. */
		 http_status_http_version_not_supported = 505, /*!< Indicates that the server does not support, or refuses to support, the protocol version that was used in the request message. */
		 http_status_variant_also_negotiates = 506, /*!< Indicates that the server has an internal configuration error: the chosen variant resource is configured to engage in transparent content negotiation itself, and is therefore not a proper end point in the negotiation process. */
		 http_status_insufficient_storage = 507, /*!< Means the method could not be performed on the resource because the server is unable to store the representation needed to successfully complete the request. */
		 http_status_loop_detected = 508, /*!< Indicates that the server terminated an operation because it encountered an infinite loop while processing a request with "Depth: infinity". [RFC 5842] */
		 http_status_not_extended = 510, /*!< The policy for accessing the resource has not been met in the request. [RFC 2774] */
		 http_status_network_authentication_required = 511, /*!< Indicates that the client needs to authenticate to gain network access. */

		 http_status_code_first_enum = http_status_continue,
		 http_status_code_last_enum = http_status_network_authentication_required

	}  http_status_code ; // typedef

	static inline bool http_status_is_informational(http_status_code code) { return (code >= 100 && code < 200); }
	static inline bool http_status_is_successful(http_status_code code) { return (code >= 200 && code < 300); }
	static inline bool http_status_is_redirection(http_status_code code) { return (code >= 300 && code < 400); }
	static inline bool http_status_is_client_error(http_status_code code) { return (code >= 400 && code < 500); }
	static inline bool http_status_is_server_error(http_status_code code) { return (code >= 500 && true /* code < 600 is always true */); }
	static inline bool http_status_is_error(http_status_code code) { return (code >= 400); }

	/*
	valstat carries the metastate to the C and C++ callers

	NOTE: using errno makes function not pure ...
	*/
	typedef struct http_status_string_valstat {
		const char* value;
		const  errno_t * status;
	} http_status_string_valstat;

	// Returns the standard HTTP reason phrase for a HTTP status code.
	// NOTE: thanks to metastate/valstat this is indeed a pure function
	DBJ_PURE_FUN
	static inline http_status_string_valstat 
		http_status_string (http_status_code code)
	{
		switch (code)
		{

#ifndef __clang__
#error 2020-08-01 DBJ -- It turns out CL extern "C" can not do compound literals yet?
#endif // ! __clang__

			/*####### 1xx - Informational #######*/
		case 100: return (http_status_string_valstat) { "Continue", 0 };
		case 101: return (http_status_string_valstat) { "Switching Protocols", 0 } ;
		case 102: return (http_status_string_valstat) { "Processing", 0 } ;
		case 103: return (http_status_string_valstat) { "Early Hints", 0 } ;

				/*####### 2xx - Successful #######*/
		case 200: return (http_status_string_valstat) { "OK", 0 } ;
		case 201: return (http_status_string_valstat) { "Created", 0 } ;
		case 202: return (http_status_string_valstat) { "Accepted", 0 } ;
		case 203: return (http_status_string_valstat) { "Non-Authoritative Information", 0 } ;
		case 204: return (http_status_string_valstat) { "No Content", 0 } ;
		case 205: return (http_status_string_valstat) { "Reset Content", 0 } ;
		case 206: return (http_status_string_valstat) { "Partial Content", 0 } ;
		case 207: return (http_status_string_valstat) { "Multi-Status", 0 } ;
		case 208: return (http_status_string_valstat) { "Already Reported", 0 } ;
		case 226: return (http_status_string_valstat) { "IM Used", 0 } ;

				/*####### 3xx - Redirection #######*/
		case 300: return (http_status_string_valstat) { "Multiple Choices", 0 } ;
		case 301: return (http_status_string_valstat) { "Moved Permanently", 0 } ;
		case 302: return (http_status_string_valstat) { "Found", 0 } ;
		case 303: return (http_status_string_valstat) { "See Other", 0 } ;
		case 304: return (http_status_string_valstat) { "Not Modified", 0 } ;
		case 305: return (http_status_string_valstat) { "Use Proxy", 0 } ;
		case 307: return (http_status_string_valstat) { "Temporary Redirect", 0 } ;
		case 308: return (http_status_string_valstat) { "Permanent Redirect", 0 } ;

				/*####### 4xx - Client Error #######*/
		case 400: return (http_status_string_valstat) { "Bad Request", 0 } ;
		case 401: return (http_status_string_valstat) { "Unauthorized", 0 } ;
		case 402: return (http_status_string_valstat) { "Payment Required", 0 } ;
		case 403: return (http_status_string_valstat) { "Forbidden", 0 } ;
		case 404: return (http_status_string_valstat) { "Not Found", 0 } ;
		case 405: return (http_status_string_valstat) { "Method Not Allowed", 0 } ;
		case 406: return (http_status_string_valstat) { "Not Acceptable", 0 } ;
		case 407: return (http_status_string_valstat) { "Proxy Authentication Required", 0 } ;
		case 408: return (http_status_string_valstat) { "Request Timeout", 0 } ;
		case 409: return (http_status_string_valstat) { "Conflict", 0 } ;
		case 410: return (http_status_string_valstat) { "Gone", 0 } ;
		case 411: return (http_status_string_valstat) { "Length Required", 0 } ;
		case 412: return (http_status_string_valstat) { "Precondition Failed", 0 } ;
		case 413: return (http_status_string_valstat) { "Payload Too Large", 0 } ;
		case 414: return (http_status_string_valstat) { "URI Too Long", 0 } ;
		case 415: return (http_status_string_valstat) { "Unsupported Media Type", 0 } ;
		case 416: return (http_status_string_valstat) { "Range Not Satisfiable", 0 } ;
		case 417: return (http_status_string_valstat) { "Expectation Failed", 0 } ;
		case 418: return (http_status_string_valstat) { "I'm a teapot", 0 } ;
		case 422: return (http_status_string_valstat) { "Unprocessable Entity", 0 } ;
		case 423: return (http_status_string_valstat) { "Locked", 0 } ;
		case 424: return (http_status_string_valstat) { "Failed Dependency", 0 } ;
		case 426: return (http_status_string_valstat) { "Upgrade Required", 0 } ;
		case 428: return (http_status_string_valstat) { "Precondition Required", 0 } ;
		case 429: return (http_status_string_valstat) { "Too Many Requests", 0 } ;
		case 431: return (http_status_string_valstat) { "Request Header Fields Too Large", 0 } ;
		case 451: return (http_status_string_valstat) { "Unavailable For Legal Reasons", 0 } ;

				/*####### 5xx - Server Error #######*/
		case 500: return (http_status_string_valstat) { "Internal Server Error", 0 } ;
		case 501: return (http_status_string_valstat) { "Not Implemented", 0 } ;
		case 502: return (http_status_string_valstat) { "Bad Gateway", 0 } ;
		case 503: return (http_status_string_valstat) { "Service Unavailable", 0 } ;
		case 504: return (http_status_string_valstat) { "Gateway Time-out", 0 } ;
		case 505: return (http_status_string_valstat) { "HTTP Version Not Supported", 0 } ;
		case 506: return (http_status_string_valstat) { "Variant Also Negotiates", 0 } ;
		case 507: return (http_status_string_valstat) { "Insufficient Storage", 0 } ;
		case 508: return (http_status_string_valstat) { "Loop Detected", 0 } ;
		case 510: return (http_status_string_valstat) { "Not Extended", 0 } ;
		case 511: return (http_status_string_valstat) { "Network Authentication Required", 0 } ;

		default: {
			static const int einval_ = 22; // invalid_argument = 22, // EINVAL
			// EINVAL is POSIX invalid_argument
			// no globals touched
			// this is a pure function
			return (http_status_string_valstat) { 0 , & einval_	};
		}

		} // switch

	} // http_status_string()

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* HTTP_STATUS_CODES_INC_ */
