errno_map = {
    "1": {
        "comment": "Operation not permitted", 
        "name": "EPERM"
    }, 
    "2": {
        "comment": "No such file or directory", 
        "name": "ENOENT"
    }, 
    "3": {
        "comment": "No such process", 
        "name": "ESRCH"
    }, 
    "4": {
        "comment": "Interrupted system call", 
        "name": "EINTR"
    }, 
    "5": {
        "comment": "I/O error", 
        "name": "EIO"
    }, 
    "6": {
        "comment": "No such device or address", 
        "name": "ENXIO"
    }, 
    "7": {
        "comment": "Argument list too long", 
        "name": "E2BIG"
    }, 
    "8": {
        "comment": "Exec format error", 
        "name": "ENOEXEC"
    }, 
    "9": {
        "comment": "Bad file number", 
        "name": "EBADF"
    }, 
    "10": {
        "comment": "No child processes", 
        "name": "ECHILD"
    }, 
    "11": {
        "comment": "Try again", 
        "name": "EAGAIN"
    }, 
    "12": {
        "comment": "Out of memory", 
        "name": "ENOMEM"
    }, 
    "13": {
        "comment": "Permission denied", 
        "name": "EACCES"
    }, 
    "14": {
        "comment": "Bad address", 
        "name": "EFAULT"
    }, 
    "15": {
        "comment": "Block device required", 
        "name": "ENOTBLK"
    }, 
    "16": {
        "comment": "Device or resource busy", 
        "name": "EBUSY"
    }, 
    "17": {
        "comment": "File exists", 
        "name": "EEXIST"
    }, 
    "18": {
        "comment": "Cross-device link", 
        "name": "EXDEV"
    }, 
    "19": {
        "comment": "No such device", 
        "name": "ENODEV"
    }, 
    "20": {
        "comment": "Not a directory", 
        "name": "ENOTDIR"
    }, 
    "21": {
        "comment": "Is a directory", 
        "name": "EISDIR"
    }, 
    "22": {
        "comment": "Invalid argument", 
        "name": "EINVAL"
    }, 
    "23": {
        "comment": "File table overflow", 
        "name": "ENFILE"
    }, 
    "24": {
        "comment": "Too many open files", 
        "name": "EMFILE"
    }, 
    "25": {
        "comment": "Not a typewriter", 
        "name": "ENOTTY"
    }, 
    "26": {
        "comment": "Text file busy", 
        "name": "ETXTBSY"
    }, 
    "27": {
        "comment": "File too large", 
        "name": "EFBIG"
    }, 
    "28": {
        "comment": "No space left on device", 
        "name": "ENOSPC"
    }, 
    "29": {
        "comment": "Illegal seek", 
        "name": "ESPIPE"
    }, 
    "30": {
        "comment": "Read-only file system", 
        "name": "EROFS"
    }, 
    "31": {
        "comment": "Too many links", 
        "name": "EMLINK"
    }, 
    "32": {
        "comment": "Broken pipe", 
        "name": "EPIPE"
    }, 
    "33": {
        "comment": "Math argument out of domain of func", 
        "name": "EDOM"
    }, 
    "34": {
        "comment": "Math result not representable", 
        "name": "ERANGE"
    }, 
    "35": {
        "comment": "Resource deadlock would occur", 
        "name": "EDEADLK"
    }, 
    "36": {
        "comment": "File name too long", 
        "name": "ENAMETOOLONG"
    }, 
    "37": {
        "comment": "No record locks available", 
        "name": "ENOLCK"
    }, 
    "38": {
        "comment": "Function not implemented", 
        "name": "ENOSYS"
    }, 
    "39": {
        "comment": "Directory not empty", 
        "name": "ENOTEMPTY"
    }, 
    "40": {
        "comment": "Too many symbolic links encountered", 
        "name": "ELOOP"
    }, 
    "42": {
        "comment": "No message of desired type", 
        "name": "ENOMSG"
    }, 
    "43": {
        "comment": "Identifier removed", 
        "name": "EIDRM"
    }, 
    "44": {
        "comment": "Channel number out of range", 
        "name": "ECHRNG"
    }, 
    "45": {
        "comment": "Level 2 not synchronized", 
        "name": "EL2NSYNC"
    }, 
    "46": {
        "comment": "Level 3 halted", 
        "name": "EL3HLT"
    }, 
    "47": {
        "comment": "Level 3 reset", 
        "name": "EL3RST"
    }, 
    "48": {
        "comment": "Link number out of range", 
        "name": "ELNRNG"
    }, 
    "49": {
        "comment": "Protocol driver not attached", 
        "name": "EUNATCH"
    }, 
    "50": {
        "comment": "No CSI structure available", 
        "name": "ENOCSI"
    }, 
    "51": {
        "comment": "Level 2 halted", 
        "name": "EL2HLT"
    }, 
    "52": {
        "comment": "Invalid exchange", 
        "name": "EBADE"
    }, 
    "53": {
        "comment": "Invalid request descriptor", 
        "name": "EBADR"
    }, 
    "54": {
        "comment": "Exchange full", 
        "name": "EXFULL"
    }, 
    "55": {
        "comment": "No anode", 
        "name": "ENOANO"
    }, 
    "56": {
        "comment": "Invalid request code", 
        "name": "EBADRQC"
    }, 
    "57": {
        "comment": "Invalid slot", 
        "name": "EBADSLT"
    }, 
    "59": {
        "comment": "Bad font file format", 
        "name": "EBFONT"
    }, 
    "60": {
        "comment": "Device not a stream", 
        "name": "ENOSTR"
    }, 
    "61": {
        "comment": "No data available", 
        "name": "ENODATA"
    }, 
    "62": {
        "comment": "Timer expired", 
        "name": "ETIME"
    }, 
    "63": {
        "comment": "Out of streams resources", 
        "name": "ENOSR"
    }, 
    "64": {
        "comment": "Machine is not on the network", 
        "name": "ENONET"
    }, 
    "65": {
        "comment": "Package not installed", 
        "name": "ENOPKG"
    }, 
    "66": {
        "comment": "Object is remote", 
        "name": "EREMOTE"
    }, 
    "67": {
        "comment": "Link has been severed", 
        "name": "ENOLINK"
    }, 
    "68": {
        "comment": "Advertise error", 
        "name": "EADV"
    }, 
    "69": {
        "comment": "Srmount error", 
        "name": "ESRMNT"
    }, 
    "70": {
        "comment": "Communication error on send", 
        "name": "ECOMM"
    }, 
    "71": {
        "comment": "Protocol error", 
        "name": "EPROTO"
    }, 
    "72": {
        "comment": "Multihop attempted", 
        "name": "EMULTIHOP"
    }, 
    "73": {
        "comment": "RFS specific error", 
        "name": "EDOTDOT"
    }, 
    "74": {
        "comment": "Not a data message", 
        "name": "EBADMSG"
    }, 
    "75": {
        "comment": "Value too large for defined data type", 
        "name": "EOVERFLOW"
    }, 
    "76": {
        "comment": "Name not unique on network", 
        "name": "ENOTUNIQ"
    }, 
    "77": {
        "comment": "File descriptor in bad state", 
        "name": "EBADFD"
    }, 
    "78": {
        "comment": "Remote address changed", 
        "name": "EREMCHG"
    }, 
    "79": {
        "comment": "Can not access a needed shared library", 
        "name": "ELIBACC"
    }, 
    "80": {
        "comment": "Accessing a corrupted shared library", 
        "name": "ELIBBAD"
    }, 
    "81": {
        "comment": ".lib section in a.out corrupted", 
        "name": "ELIBSCN"
    }, 
    "82": {
        "comment": "Attempting to link in too many shared libraries", 
        "name": "ELIBMAX"
    }, 
    "83": {
        "comment": "Cannot exec a shared library directly", 
        "name": "ELIBEXEC"
    }, 
    "84": {
        "comment": "Illegal byte sequence", 
        "name": "EILSEQ"
    }, 
    "85": {
        "comment": "Interrupted system call should be restarted", 
        "name": "ERESTART"
    }, 
    "86": {
        "comment": "Streams pipe error", 
        "name": "ESTRPIPE"
    }, 
    "87": {
        "comment": "Too many users", 
        "name": "EUSERS"
    }, 
    "88": {
        "comment": "Socket operation on non-socket", 
        "name": "ENOTSOCK"
    }, 
    "89": {
        "comment": "Destination address required", 
        "name": "EDESTADDRREQ"
    }, 
    "90": {
        "comment": "Message too long", 
        "name": "EMSGSIZE"
    }, 
    "91": {
        "comment": "Protocol wrong type for socket", 
        "name": "EPROTOTYPE"
    }, 
    "92": {
        "comment": "Protocol not available", 
        "name": "ENOPROTOOPT"
    }, 
    "93": {
        "comment": "Protocol not supported", 
        "name": "EPROTONOSUPBOARD"
    }, 
    "94": {
        "comment": "Socket type not supported", 
        "name": "ESOCKTNOSUPBOARD"
    }, 
    "95": {
        "comment": "Operation not supported on transport endpoint", 
        "name": "EOPNOTSUPP"
    }, 
    "96": {
        "comment": "Protocol family not supported", 
        "name": "EPFNOSUPBOARD"
    }, 
    "97": {
        "comment": "Address family not supported by protocol", 
        "name": "EAFNOSUPBOARD"
    }, 
    "98": {
        "comment": "Address already in use", 
        "name": "EADDRINUSE"
    }, 
    "99": {
        "comment": "Cannot assign requested address", 
        "name": "EADDRNOTAVAIL"
    }, 
    "100": {
        "comment": "Network is down", 
        "name": "ENETDOWN"
    }, 
    "101": {
        "comment": "Network is unreachable", 
        "name": "ENETUNREACH"
    }, 
    "102": {
        "comment": "Network dropped connection because of reset", 
        "name": "ENETRESET"
    }, 
    "103": {
        "comment": "Software caused connection abort", 
        "name": "ECONNABORTED"
    }, 
    "104": {
        "comment": "Connection reset by peer", 
        "name": "ECONNRESET"
    }, 
    "105": {
        "comment": "No buffer space available", 
        "name": "ENOBUFS"
    }, 
    "106": {
        "comment": "Transport endpoint is already connected", 
        "name": "EISCONN"
    }, 
    "107": {
        "comment": "Transport endpoint is not connected", 
        "name": "ENOTCONN"
    }, 
    "108": {
        "comment": "Cannot send after transport endpoint shutdown", 
        "name": "ESHUTDOWN"
    }, 
    "109": {
        "comment": "Too many references: cannot splice", 
        "name": "ETOOMANYREFS"
    }, 
    "110": {
        "comment": "Connection timed out", 
        "name": "ETIMEDOUT"
    }, 
    "111": {
        "comment": "Connection refused", 
        "name": "ECONNREFUSED"
    }, 
    "112": {
        "comment": "Host is down", 
        "name": "EHOSTDOWN"
    }, 
    "113": {
        "comment": "No route to host", 
        "name": "EHOSTUNREACH"
    }, 
    "114": {
        "comment": "Operation already in progress", 
        "name": "EALREADY"
    }, 
    "115": {
        "comment": "Operation now in progress", 
        "name": "EINPROGRESS"
    }, 
    "116": {
        "comment": "Stale NFS file handle", 
        "name": "ESTALE"
    }, 
    "117": {
        "comment": "Structure needs cleaning", 
        "name": "EUCLEAN"
    }, 
    "118": {
        "comment": "Not a XENIX named type file", 
        "name": "ENOTNAM"
    }, 
    "119": {
        "comment": "No XENIX sems available", 
        "name": "ENAVAIL"
    }, 
    "120": {
        "comment": "Is a named type file", 
        "name": "EISNAM"
    }, 
    "121": {
        "comment": "Remote I/O error", 
        "name": "EREMOTEIO"
    }, 
    "122": {
        "comment": "Quota exceeded", 
        "name": "EDQUOT"
    }, 
    "123": {
        "comment": "No medium found", 
        "name": "ENOMEDIUM"
    }, 
    "124": {
        "comment": "Wrong medium type", 
        "name": "EMEDIUMTYPE"
    }, 
    "125": {
        "comment": "Operation Canceled", 
        "name": "ECANCELED"
    }, 
    "126": {
        "comment": "Required key not available", 
        "name": "ENOKEY"
    }, 
    "127": {
        "comment": "Key has expired", 
        "name": "EKEYEXPIRED"
    }, 
    "128": {
        "comment": "Key has been revoked", 
        "name": "EKEYREVOKED"
    }, 
    "129": {
        "comment": "Key was rejected by service", 
        "name": "EKEYREJECTED"
    }, 
    "1000": {
        "comment": "Stack corrupt.", 
        "name": "ESTACK"
    }, 
    "1001": {
        "comment": "Watchdog timeout.", 
        "name": "EWATCHDOGTIMEOUT"
    }
}