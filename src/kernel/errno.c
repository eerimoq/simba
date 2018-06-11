/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

static FAR const char eperm_string[] = "Operation not permitted";
static FAR const char enoent_string[] = "No such file or directory";
static FAR const char esrch_string[] = "No such process";
static FAR const char eintr_string[] = "Interrupted system call";
static FAR const char eio_string[] = "I/O error";
static FAR const char enxio_string[] = "No such device or address";
static FAR const char e2big_string[] = "Argument list too long";
static FAR const char enoexec_string[] = "Exec format error";
static FAR const char ebadf_string[] = "Bad file number";
static FAR const char echild_string[] = "No child processes";
static FAR const char eagain_string[] = "Try again";
static FAR const char enomem_string[] = "Out of memory";
static FAR const char eacces_string[] = "Permission denied";
static FAR const char efault_string[] = "Bad address";
static FAR const char enotblk_string[] = "Block device required";
static FAR const char ebusy_string[] = "Device or resource busy";
static FAR const char eexist_string[] = "File exists";
static FAR const char exdev_string[] = "Cross-device link";
static FAR const char enodev_string[] = "No such device";
static FAR const char enotdir_string[] = "Not a directory";
static FAR const char eisdir_string[] = "Is a directory";
static FAR const char einval_string[] = "Invalid argument";
static FAR const char enfile_string[] = "File table overflow";
static FAR const char emfile_string[] = "Too many open files";
static FAR const char enotty_string[] = "Not a typewriter";
static FAR const char etxtbsy_string[] = "Text file busy";
static FAR const char efbig_string[] = "File too large";
static FAR const char enospc_string[] = "No space left on device";
static FAR const char espipe_string[] = "Illegal seek";
static FAR const char erofs_string[] = "Read-only file system";
static FAR const char emlink_string[] = "Too many links";
static FAR const char epipe_string[] = "Broken pipe";
static FAR const char edom_string[] = "Math argument out of domain of func";
static FAR const char erange_string[] = "Math result not representable";
static FAR const char edeadlk_string[] = "Resource deadlock would occur";
static FAR const char enametoolong_string[] = "File name too long";
static FAR const char enolck_string[] = "No record locks available";
static FAR const char enosys_string[] = "Function not implemented";
static FAR const char enotempty_string[] = "Directory not empty";
static FAR const char eloop_string[] = "Too many symbolic links encountered";
static FAR const char enomsg_string[] = "No message of desired type";
static FAR const char eidrm_string[] = "Identifier removed";
static FAR const char echrng_string[] = "Channel number out of range";
static FAR const char el2nsync_string[] = "Level 2 not synchronized";
static FAR const char el3hlt_string[] = "Level 3 halted";
static FAR const char el3rst_string[] = "Level 3 reset";
static FAR const char elnrng_string[] = "Link number out of range";
static FAR const char eunatch_string[] = "Protocol driver not attached";
static FAR const char enocsi_string[] = "No CSI structure available";
static FAR const char el2hlt_string[] = "Level 2 halted";
static FAR const char ebade_string[] = "Invalid exchange";
static FAR const char ebadr_string[] = "Invalid request descriptor";
static FAR const char exfull_string[] = "Exchange full";
static FAR const char enoano_string[] = "No anode";
static FAR const char ebadrqc_string[] = "Invalid request code";
static FAR const char ebadslt_string[] = "Invalid slot";
static FAR const char ebfont_string[] = "Bad font file format";
static FAR const char enostr_string[] = "Device not a stream";
static FAR const char enodata_string[] = "No data available";
static FAR const char etime_string[] = "Timer expired";
static FAR const char enosr_string[] = "Out of streams resources";
static FAR const char enonet_string[] = "Machine is not on the network";
static FAR const char enopkg_string[] = "Package not installed";
static FAR const char eremote_string[] = "Object is remote";
static FAR const char enolink_string[] = "Link has been severed";
static FAR const char eadv_string[] = "Advertise error";
static FAR const char esrmnt_string[] = "Srmount error";
static FAR const char ecomm_string[] = "Communication error on send";
static FAR const char eproto_string[] = "Protocol error";
static FAR const char emultihop_string[] = "Multihop attempted";
static FAR const char edotdot_string[] = "RFS specific error";
static FAR const char ebadmsg_string[] = "Not a data message";
static FAR const char eoverflow_string[] = "Value too large for defined data type";
static FAR const char enotuniq_string[] = "Name not unique on network";
static FAR const char ebadfd_string[] = "File descriptor in bad state";
static FAR const char eremchg_string[] = "Remote address changed";
static FAR const char elibacc_string[] = "Can not access a needed shared library";
static FAR const char elibbad_string[] = "Accessing a corrupted shared library";
static FAR const char elibscn_string[] = ".lib section in a.out corrupted";
static FAR const char elibmax_string[] = "Attempting to link in too many shared libraries";
static FAR const char elibexec_string[] = "Cannot exec a shared library directly";
static FAR const char eilseq_string[] = "Illegal byte sequence";
static FAR const char erestart_string[] = "Interrupted system call should be restarted";
static FAR const char estrpipe_string[] = "Streams pipe error";
static FAR const char eusers_string[] = "Too many users";
static FAR const char enotsock_string[] = "Socket operation on non-socket";
static FAR const char edestaddrreq_string[] = "Destination address required";
static FAR const char emsgsize_string[] = "Message too long";
static FAR const char eprototype_string[] = "Protocol wrong type for socket";
static FAR const char enoprotoopt_string[] = "Protocol not available";
static FAR const char eprotonosupboard_string[] = "Protocol not supported";
static FAR const char esocktnosupboard_string[] = "Socket type not supported";
static FAR const char eopnotsupp_string[] = "Operation not supported on transport endpoint";
static FAR const char epfnosupboard_string[] = "Protocol family not supported";
static FAR const char eafnosupboard_string[] = "Address family not supported by protocol";
static FAR const char eaddrinuse_string[] = "Address already in use";
static FAR const char eaddrnotavail_string[] = "Cannot assign requested address";
static FAR const char enetdown_string[] = "Network is down";
static FAR const char enetunreach_string[] = "Network is unreachable";
static FAR const char enetreset_string[] = "Network dropped connection because of reset";
static FAR const char econnaborted_string[] = "Software caused connection abort";
static FAR const char econnreset_string[] = "Connection reset by peer";
static FAR const char enobufs_string[] = "No buffer space available";
static FAR const char eisconn_string[] = "Transport endpoint is already connected";
static FAR const char enotconn_string[] = "Transport endpoint is not connected";
static FAR const char eshutdown_string[] = "Cannot send after transport endpoint shutdown";
static FAR const char etoomanyrefs_string[] = "Too many references: cannot splice";
static FAR const char etimedout_string[] = "Connection timed out";
static FAR const char econnrefused_string[] = "Connection refused";
static FAR const char ehostdown_string[] = "Host is down";
static FAR const char ehostunreach_string[] = "No route to host";
static FAR const char ealready_string[] = "Operation already in progress";
static FAR const char einprogress_string[] = "Operation now in progress";
static FAR const char estale_string[] = "Stale NFS file handle";
static FAR const char euclean_string[] = "Structure needs cleaning";
static FAR const char enotnam_string[] = "Not a XENIX named type file";
static FAR const char enavail_string[] = "No XENIX sems available";
static FAR const char eisnam_string[] = "Is a named type file";
static FAR const char eremoteio_string[] = "Remote I/O error";
static FAR const char edquot_string[] = "Quota exceeded";
static FAR const char enomedium_string[] = "No medium found";
static FAR const char emediumtype_string[] = "Wrong medium type";
static FAR const char ecanceled_string[] = "Operation Canceled";
static FAR const char enokey_string[] = "Required key not available";
static FAR const char ekeyexpired_string[] = "Key has expired";
static FAR const char ekeyrevoked_string[] = "Key has been revoked";
static FAR const char ekeyrejected_string[] = "Key was rejected by service";

static FAR const char* FAR const strings[] = {
    eperm_string,
    enoent_string,
    esrch_string,
    eintr_string,
    eio_string,
    enxio_string,
    e2big_string,
    enoexec_string,
    ebadf_string,
    echild_string,
    eagain_string,
    enomem_string,
    eacces_string,
    efault_string,
    enotblk_string,
    ebusy_string,
    eexist_string,
    exdev_string,
    enodev_string,
    enotdir_string,
    eisdir_string,
    einval_string,
    enfile_string,
    emfile_string,
    enotty_string,
    etxtbsy_string,
    efbig_string,
    enospc_string,
    espipe_string,
    erofs_string,
    emlink_string,
    epipe_string,
    edom_string,
    erange_string,
    edeadlk_string,
    enametoolong_string,
    enolck_string,
    enosys_string,
    enotempty_string,
    eloop_string,
    eagain_string,
    enomsg_string,
    eidrm_string,
    echrng_string,
    el2nsync_string,
    el3hlt_string,
    el3rst_string,
    elnrng_string,
    eunatch_string,
    enocsi_string,
    el2hlt_string,
    ebade_string,
    ebadr_string,
    exfull_string,
    enoano_string,
    ebadrqc_string,
    ebadslt_string,
    edeadlk_string,
    ebfont_string,
    enostr_string,
    enodata_string,
    etime_string,
    enosr_string,
    enonet_string,
    enopkg_string,
    eremote_string,
    enolink_string,
    eadv_string,
    esrmnt_string,
    ecomm_string,
    eproto_string,
    emultihop_string,
    edotdot_string,
    ebadmsg_string,
    eoverflow_string,
    enotuniq_string,
    ebadfd_string,
    eremchg_string,
    elibacc_string,
    elibbad_string,
    elibscn_string,
    elibmax_string,
    elibexec_string,
    eilseq_string,
    erestart_string,
    estrpipe_string,
    eusers_string,
    enotsock_string,
    edestaddrreq_string,
    emsgsize_string,
    eprototype_string,
    enoprotoopt_string,
    eprotonosupboard_string,
    esocktnosupboard_string,
    eopnotsupp_string,
    epfnosupboard_string,
    eafnosupboard_string,
    eaddrinuse_string,
    eaddrnotavail_string,
    enetdown_string,
    enetunreach_string,
    enetreset_string,
    econnaborted_string,
    econnreset_string,
    enobufs_string,
    eisconn_string,
    enotconn_string,
    eshutdown_string,
    etoomanyrefs_string,
    etimedout_string,
    econnrefused_string,
    ehostdown_string,
    ehostunreach_string,
    ealready_string,
    einprogress_string,
    estale_string,
    euclean_string,
    enotnam_string,
    enavail_string,
    eisnam_string,
    eremoteio_string,
    edquot_string,
    enomedium_string,
    emediumtype_string,
    ecanceled_string,
    enokey_string,
    ekeyexpired_string,
    ekeyrevoked_string,
    ekeyrejected_string
};

static FAR const char estack_string[] = "Stack corrupt";
static FAR const char ebtassert_string[] = "Test assertion";
static FAR const char eassert_string[] = "Assertion";
static FAR const char enocommand_string[] = "Command not found";
static FAR const char enotmounted_string[] = "Resource not mounted";
static FAR const char ekeynotfound_string[] = "Key not found";
static FAR const char ebadvalue_string[] = "Bad value";

static FAR const char *FAR const simba_strings[] = {
    estack_string,
    ebtassert_string,
    eassert_string,
    enocommand_string,
    enotmounted_string,
    ekeynotfound_string,
    ebadvalue_string
};

far_string_t errno_as_string(int errno)
{
    if (errno < 0) {
        errno *= -1;
    }

    if ((errno >= EPERM) && (errno <= EKEYREJECTED)) {
        return strings[errno - EPERM];
    } else if ((errno >= ESTACK) && (errno <= EBADVALUE)) {
        return simba_strings[errno - ESTACK];
    } else {
        return (NULL);
    }
}
