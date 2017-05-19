/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
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


#include "mbedtls/config.h"

#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_cookie.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"
#include "mbedtls/timing.h"
#include "mbedtls/x509_csr.h"

#include "mbedtls/pk.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/rsa.h"

#define SSL_MODULE_ALLOCATED_SSL 0
#define SSL_MODULE_ALLOCATED_CONF 1
#define SSL_MODULE_ALLOCATED_CERT 2
#define SSL_MODULE_ALLOCATED_KEY 3
#define SSL_MODULE_ALLOCATED_CA_CERTS 4

struct module_t {
    char initialized;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt cert;
    mbedtls_pk_context key;
    mbedtls_x509_crt ca_certs;
    mbedtls_timing_delay_context timer;
		char allocated[5];
};

static struct module_t module;
void ssl_module_reset()
{
    if(module.initialized == 0)
    {
            return ;
    }
    //these are always initialized in module.init 
    mbedtls_entropy_free(&module.entropy);
    mbedtls_ctr_drbg_free(&module.ctr_drbg);
    
    //sanity checks 
    if(module.allocated[SSL_MODULE_ALLOCATED_SSL])
            mbedtls_ssl_free(&module.ssl);
    if(module.allocated[SSL_MODULE_ALLOCATED_CONF])
            mbedtls_ssl_config_free(&module.conf);
    if(module.allocated[SSL_MODULE_ALLOCATED_CERT])
            mbedtls_x509_crt_free(&module.cert);
    if(module.allocated[SSL_MODULE_ALLOCATED_KEY])
            mbedtls_pk_free(&module.key);
    if(module.allocated[SSL_MODULE_ALLOCATED_CA_CERTS])
            mbedtls_x509_crt_free(&module.ca_certs);
    memset(&module.timer,0,sizeof(module.timer));
    module.initialized = 0;
    memset(module.allocated,0,sizeof(module.allocated));
    ssl_module_init();
}
static void *alloc_ssl(void)
{
    if (module.allocated[SSL_MODULE_ALLOCATED_SSL] == 1) {
				printf("alloc_ssl returns NULL \n");
        return (NULL);
    }
    module.allocated[SSL_MODULE_ALLOCATED_SSL]=1;
    return (&module.ssl);
}

static void free_ssl(void *ssl_p)
{
    module.allocated[SSL_MODULE_ALLOCATED_SSL] = 0;
}

static void *alloc_conf(void)
{
    if (module.allocated[SSL_MODULE_ALLOCATED_CONF] == 1) {
        return (NULL);
    }
    module.allocated[SSL_MODULE_ALLOCATED_CONF] = 1;

    return (&module.conf);
}

static void free_conf(void *conf_p)
{
    module.allocated[SSL_MODULE_ALLOCATED_CONF] = 0;
}

static int ssl_send(void *ctx_p,
                    const unsigned char *buf_p,
                    size_t len)
{
    return (socket_write(ctx_p, buf_p, len));
}

static int ssl_recv(void *ctx_p,
                    unsigned char *buf_p,
                    size_t len )
{
    return (socket_read(ctx_p, buf_p, len));
}

int ssl_module_init()
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;
    mbedtls_entropy_init(&module.entropy);
    mbedtls_ctr_drbg_init(&module.ctr_drbg);

    /* Random generator seed initialization. */
    if (mbedtls_ctr_drbg_seed(&module.ctr_drbg,
                              mbedtls_entropy_func,
                              &module.entropy,
                              NULL,
                              0) != 0) {
        return (-1);
    }
		printf("done reinitializing\n");
    return (0);
}

int ssl_context_init(struct ssl_context_t *self_p,
                     enum ssl_protocol_t protocol)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(module.initialized == 1, EINVAL);
    self_p->protocol = protocol;
		
    /* Allocate the mbedTLS SSL config datastructure. */
    self_p->conf_p = alloc_conf();

    if (self_p->conf_p == NULL) {
        return (-1);
    }

    mbedtls_ssl_config_init(self_p->conf_p);
    mbedtls_ssl_conf_rng(self_p->conf_p,
                         mbedtls_ctr_drbg_random,
                         &module.ctr_drbg);

    self_p->server_side = -1;
    self_p->verify_mode = -1;

    return (0);
}

int ssl_context_destroy(struct ssl_context_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(self_p->conf_p != NULL, EINVAL);
		module.initialized = 0 ;
    free_conf(self_p->conf_p);
    return (0);
}

int ssl_context_load_cert_chain(struct ssl_context_t *self_p,
                                const char *cert_p,
                                const char *key_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(self_p->conf_p != NULL, EINVAL);
    ASSERTN(cert_p != NULL, EINVAL);
		module.allocated[SSL_MODULE_ALLOCATED_CERT]=1; 
		module.allocated[SSL_MODULE_ALLOCATED_KEY]=1;
    mbedtls_x509_crt_init(&module.cert);
    mbedtls_pk_init(&module.key);

    /* Parse the certificate. */
    if (mbedtls_x509_crt_parse(&module.cert,
                               (unsigned char *)cert_p,
                               strlen(cert_p) + 1) != 0) {
        return (-1);
    }

    /* Parse the key if present. */
    if (key_p != NULL) {
        if (mbedtls_pk_parse_key(&module.key,
                                 (unsigned char *)key_p,
                                 strlen(key_p) + 1,
                                 NULL,
                                 0) != 0) {
            return (-1);
        }
    }

    if (mbedtls_ssl_conf_own_cert(self_p->conf_p,
                                  &module.cert,
                                  &module.key) != 0) {
        return (-1);
    }

    return (0);
}

int ssl_context_load_verify_location(struct ssl_context_t *self_p,
                                     const char *ca_certs_p)
{
    /* Parse the CA certificate(s). */
		module.allocated[SSL_MODULE_ALLOCATED_CA_CERTS]=1;
    if (mbedtls_x509_crt_parse(&module.ca_certs,
                               (unsigned char *)ca_certs_p,
                               strlen(ca_certs_p) + 1) != 0) {
        return (-1);
    }

    mbedtls_ssl_conf_ca_chain(self_p->conf_p,
                              &module.ca_certs,
                              NULL);

    return (0);
}

int ssl_context_set_verify_mode(struct ssl_context_t *self_p,
                                enum ssl_verify_mode_t mode)
{
    ASSERT(MBEDTLS_SSL_VERIFY_NONE == ssl_verify_mode_cert_none_t);
    ASSERT(MBEDTLS_SSL_VERIFY_REQUIRED == ssl_verify_mode_cert_required_t);

    self_p->verify_mode = mode;

    return (0);
}

static void my_f_debug( void *ctx, int level,
                      const char *file, int line, const char *str )
{
    ((void) level);
    fprintf( (FILE *) ctx, "%s:%04d: %s", file, line, str );
    fflush(  (FILE *) ctx  );
}

int ssl_socket_open(struct ssl_socket_t *self_p,
                    struct ssl_context_t *context_p,
                    void *socket_p,
                    int flags,
                    const char *server_hostname_p)
{
    ASSERTN(module.initialized == 1, EINVAL);
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(context_p != NULL, EINVAL);
    ASSERTN(context_p->conf_p != NULL, EINVAL);
    ASSERTN(socket_p != NULL, EINVAL);
    int res;
    int authmode;
    int server_side;
   // mbedtls_ssl_conf_dbg(context_p->conf_p, my_f_debug, stdout);
    mbedtls_ssl_conf_dbg(context_p->conf_p, my_f_debug, stdout);
    server_side = (flags & SSL_SOCKET_SERVER_SIDE);
    
    /* This implementation can only handle a single socket mode,
       server or client, in a context. Which mode to configure is
       first known in this function, so the context cannot be
       configured earlier. */
    if (context_p->server_side == -1) {
        if (mbedtls_ssl_config_defaults(context_p->conf_p,
                                        server_side,
                                        MBEDTLS_SSL_TRANSPORT_STREAM,
                                        MBEDTLS_SSL_PRESET_DEFAULT) != 0) {
						printf("A crapat la if1\n");
            return (-1);
        }

        /* Use mbedTLS default values (none for servers, required for
           clients. */
        if (context_p->verify_mode != -1) {
            mbedtls_ssl_conf_authmode(context_p->conf_p, context_p->verify_mode);
        }

        context_p->server_side = server_side;
    } else if (context_p->server_side != server_side) {
				printf("A crapat la if2\n");
        return (-1);
    }

    self_p->socket_p = socket_p;

    /* Allocate the mbedTLS datastructures. */
    self_p->ssl_p = alloc_ssl();

    if (self_p->ssl_p == NULL) {
				printf("A crapat la if3\n");
        return (-1);
    }

    chan_init(&self_p->base,
              (chan_read_fn_t)ssl_socket_read,
              (chan_write_fn_t)ssl_socket_write,
              (chan_size_fn_t)ssl_socket_size);

    /* Inilialize the SSL session. */
    mbedtls_ssl_init(self_p->ssl_p);

    res = mbedtls_ssl_setup(self_p->ssl_p, &module.conf);

    if (res != 0) {
        goto err1;
    }

    mbedtls_ssl_set_bio(self_p->ssl_p,
                        socket_p,
                        ssl_send,
                        ssl_recv,
                        NULL);

    /* Server hostname for client side sockets. */
    if (server_side == 0) {
        if (server_hostname_p != NULL) {
            if (mbedtls_ssl_set_hostname(self_p->ssl_p,
                                         server_hostname_p) != 0) {
                goto err2;
            }
        }
    }
    
    /* Perform the handshake with the remote peer. */
    res = mbedtls_ssl_handshake(self_p->ssl_p);

    if (res != 0) {
        goto err2;
    }

    /* Verify the peer certificate if optional and present. */
    authmode = ((mbedtls_ssl_config *)context_p->conf_p)->authmode;

    if (authmode == MBEDTLS_SSL_VERIFY_OPTIONAL) {
        if (mbedtls_ssl_get_verify_result(self_p->ssl_p) != 0) {
            std_printf(FSTR("Verification of the peer certificate failed.\r\n"));
            res = -1;
            goto err2;
        }
    }

    return (0);

 err2:
    mbedtls_ssl_free(self_p->ssl_p);
 err1:
    free_ssl(self_p->ssl_p);

    return (res);
}

int ssl_socket_close(struct ssl_socket_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(self_p->ssl_p != NULL, EINVAL);

    mbedtls_ssl_close_notify(self_p->ssl_p);
    mbedtls_ssl_free(self_p->ssl_p);
    free_ssl(self_p->ssl_p);

    return (0);
}

ssize_t ssl_socket_write(struct ssl_socket_t *self_p,
                         const void *buf_p,
                         size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(self_p->ssl_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    return (mbedtls_ssl_write(self_p->ssl_p, buf_p, size));
}

ssize_t ssl_socket_read(struct ssl_socket_t *self_p,
                        void *buf_p,
                        size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(self_p->ssl_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    return (mbedtls_ssl_read(self_p->ssl_p, buf_p, size));
}

ssize_t ssl_socket_size(struct ssl_socket_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(self_p->ssl_p != NULL, EINVAL);

    return (mbedtls_ssl_get_bytes_avail(self_p->ssl_p)
            + chan_size(self_p->socket_p));
}

const char *ssl_socket_get_server_hostname(struct ssl_socket_t *self_p)
{
    return (((mbedtls_ssl_context *)self_p->ssl_p)->hostname);
}

int ssl_socket_get_cipher(struct ssl_socket_t *self_p,
                          const char **cipher_pp,
                          const char **protocol_pp,
                          int *number_of_secret_bits_p)
{
    *cipher_pp = mbedtls_ssl_get_ciphersuite(self_p->ssl_p);
    *protocol_pp = mbedtls_ssl_get_version(self_p->ssl_p);
    *number_of_secret_bits_p = -1;
    
    return (0);
}


// KEY GENERATION
static mbedtls_pk_context global_key; 
/*workaround for not including additional headers in module_ssl.c .
we free the memory used by this when finishing creating the certificate*/
static mbedtls_pk_context generate_key(unsigned char* output_buf);
static int write_private_key(unsigned char* output_buf);
int ssl_context_create_private_key(unsigned char* output_buf)
{
	global_key = generate_key(output_buf);
	return write_private_key(output_buf );
}

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf     printf
#endif

#if defined(MBEDTLS_PK_WRITE_C) && \
	defined(MBEDTLS_ENTROPY_C) && defined(MBEDTLS_CTR_DRBG_C)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DEV_RANDOM_THRESHOLD        32
#endif

#if defined(MBEDTLS_ECP_C)
#define DFL_EC_CURVE            mbedtls_ecp_curve_list()->grp_id
#else
#define DFL_EC_CURVE            0
#endif

#if !defined(_WIN32) && defined(MBEDTLS_FS_IO)
#define USAGE_DEV_RANDOM \
     "    use_dev_random=0|1    default: 0\n"
#else
#define USAGE_DEV_RANDOM ""
#endif 

#define FORMAT_PEM              0
#define FORMAT_DER              1

#define DFL_TYPE                MBEDTLS_PK_RSA
#define DFL_RSA_KEYSIZE         1024
#define DFL_FORMAT              FORMAT_PEM
#define DFL_USE_DEV_RANDOM      0
 
/*
type=rsa|ec           default: rsa\n"          
rsa_keysize=%%d        default: 1024\n"        
ec_curve=%%s           see below\n"            
format=pem|der        default: pem\n"          
USAGE_DEV_RANDOM                                    

*/
struct options_key
{
	int type;                   /* the type of key to generate          */
	int rsa_keysize;            /* length of key in bits                */
	int ec_curve;               /* curve identifier for EC keys         */
	int format;                 /* the output format to use             */
	int use_dev_random;         /* use /dev/random as entropy source    */
} opt_key;
 
static int write_private_key( unsigned char* output_buf)
{
	int ret;
	int len = 0;
	if( opt_key.format == FORMAT_PEM )
	{
		if( ( ret = mbedtls_pk_write_key_pem(&global_key, output_buf, 1024 ) ) != 0 )
            return( ret );
        len = strlen( (char *) output_buf );
	}
    //todo : support FORMAT_DER
	return len;

}
 
static mbedtls_pk_context generate_key(unsigned char* output_buf)
{
	int ret = 1;
	mbedtls_pk_context key;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	const char *pers = "gen_key";
	mbedtls_pk_init( &key );
	mbedtls_ctr_drbg_init( &ctr_drbg );

    
	opt_key.type                = DFL_TYPE;
	opt_key.rsa_keysize         = DFL_RSA_KEYSIZE;
	opt_key.ec_curve            = DFL_EC_CURVE;
	opt_key.format              = DFL_FORMAT;
	opt_key.use_dev_random      = DFL_USE_DEV_RANDOM;
	mbedtls_entropy_init( &entropy );
	if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
														 (const unsigned char *) pers,
														 strlen( pers ) ) ) != 0 )
	{
        mbedtls_printf( "Certificate creation failed\n  ! mbedtls_ctr_drbg_seed returned -0x%04x\n", -ret );
        goto exit;
	}


	if( ( ret = mbedtls_pk_setup( &key, mbedtls_pk_info_from_type( opt_key.type ) ) ) != 0 )
	{
        mbedtls_printf( "Certificate creation  failed\n  !  mbedtls_pk_setup returned -0x%04x", -ret );
        goto exit;
	}

#if defined(MBEDTLS_RSA_C) && defined(MBEDTLS_GENPRIME)
	if( opt_key.type == MBEDTLS_PK_RSA )
	{
        ret = mbedtls_rsa_gen_key( mbedtls_pk_rsa( key ), mbedtls_ctr_drbg_random, &ctr_drbg,
                                             opt_key.rsa_keysize, 65537 );
        if( ret != 0 )
        {
                mbedtls_printf( "Certificate creation  failed\n  !  mbedtls_rsa_gen_key returned -0x%04x", -ret );
                goto exit;
        }
	}
	else
#endif /* MBEDTLS_RSA_C */
#if defined(MBEDTLS_ECP_C)
	if( opt_key.type == MBEDTLS_PK_ECKEY )
	{
        ret = mbedtls_ecp_gen_key( opt_key.ec_curve, mbedtls_pk_ec( key ),
                                            mbedtls_ctr_drbg_random, &ctr_drbg );
        if( ret != 0 )
        {
                mbedtls_printf( "Certificate creation  failed\n  !  mbedtls_rsa_gen_key returned -0x%04x", -ret );
                goto exit;
        }
	}
	else
#endif /* MBEDTLS_ECP_C */
	{
        mbedtls_printf( "Certificate creation  failed\n  !  key type not supported\n" );
        goto exit;
	}
  mbedtls_entropy_free( &entropy );
  mbedtls_ctr_drbg_free( &ctr_drbg );
exit:

	return(key) ;
}


// CERTIFICATE GENERATION
#if defined(MBEDTLS_X509_CSR_PARSE_C)
#define USAGE_CSR                                                           \
    "    request_file=%%s     default: (empty)\n"                           \
    "                        If request_file is specified, subject_key,\n"  \
    "                        subject_pwd and subject_name are ignored!\n"
#else
#define USAGE_CSR ""
#endif /* MBEDTLS_X509_CSR_PARSE_C */

#define DFL_ISSUER_CRT          ""
#define DFL_REQUEST_FILE        ""
#define DFL_ISSUER_KEY          "ca.key"
#define DFL_SUBJECT_PWD         ""
#define DFL_ISSUER_PWD          ""
#define DFL_SUBJECT_NAME        "CN=Cert,O=mbed TLS,C=UK"
#define DFL_ISSUER_NAME         "CN=CA,O=mbed TLS,C=UK"
#define DFL_NOT_BEFORE          "20010101000000"
#define DFL_NOT_AFTER           "20301231235959"
#define DFL_SERIAL              "1"
#define DFL_SELFSIGN            1
#define DFL_IS_CA               0
#define DFL_MAX_PATHLEN         -1
#define DFL_KEY_USAGE           0
#define DFL_NS_CERT_TYPE        0

/*
USAGE_CSR                                           
    subject_key=%%s      default: subject.key 
    subject_pwd=%%s      default: (empty)     
    subject_name=%%s     default: CN=Cert,O=mbed TLS,C=UK
    
    issuer_crt=%%s       default: (empty)       
                        If issuer_crt is specified, issuer_name is  
                        ignored!              
    issuer_name=%%s      default: CN=CA,O=mbed TLS,C=UK     
                                                
    selfsign=%%d         default: 1 (true)     
                        If selfsign is enabled, issuer_name and
                        issuer_key are required (issuer_crt and
                        subject_* are ignored
    issuer_key=%%s       default: ca.key        
    issuer_pwd=%%s       default: (empty)     
    output_file=%%s      default: cert.crt     
    serial=%%s           default: 1            
    not_before=%%s       default: 20010101000000
    not_after=%%s        default: 20301231235959
    is_ca=%%d            default: 0 (disabled)  
    max_pathlen=%%d      default: -1 (none)     
    key_usage=%%s        default: (empty)       
                        Comma-separated-list of values:
                          digital_signature     
                          non_repudiation       
                          key_encipherment      
                          data_encipherment     
                          key_agreement         
                          key_cert_sign  
                          crl_sign            
    ns_cert_type=%%s     default: (empty)      
                        Comma-separated-list of values:     
                          ssl_client           
                          ssl_server           
                          email               
                          object_signing       
                          ssl_ca               
                          email_ca              
                          object_signing_ca  
*/
struct options_cert
{
	const char *issuer_crt;     /* filename of the issuer certificate   */
	const char *request_file;   /* filename of the certificate request  */
	const char *issuer_key;     /* filename of the issuer key file      */
	const char *subject_pwd;    /* password for the subject key file    */
	const char *issuer_pwd;     /* password for the issuer key file     */
	const char *output_file;    /* where to store the constructed key file  */
	const char *subject_name;   /* subject name for certificate         */
	const char *issuer_name;    /* issuer name for certificate          */
	const char *not_before;     /* validity period not before           */
	const char *not_after;      /* validity period not after            */
	const char *serial;         /* serial number string                 */
	int selfsign;               /* selfsign the certificate             */
	int is_ca;                  /* is a CA certificate                  */
	int max_pathlen;            /* maximum CA path length               */
	unsigned char key_usage;    /* key usage flags                      */
	unsigned char ns_cert_type; /* NS cert type                         */
} opt_cert;

int write_certificate( mbedtls_x509write_cert *crt,
                       int (*f_rng)(void *, unsigned char *, size_t),
                       void *p_rng, unsigned char* output_buf )
{
	int ret ;
	if( (ret = mbedtls_x509write_crt_pem( crt, output_buf, 1024, f_rng, p_rng ) ) < 0 )
		return ret;
	return strlen((char *)output_buf);
}
int create_certificate_from_key(unsigned char *cert_buf)
{ 
	 
    int ret = 0;
    mbedtls_x509_crt issuer_crt;
		mbedtls_pk_context *issuer_key = &global_key; 
#if defined(MBEDTLS_X509_CSR_PARSE_C)
    mbedtls_x509_csr csr;
#endif
    mbedtls_x509write_cert crt;
    mbedtls_mpi serial;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    const char *pers = "crt example app";
    
    mbedtls_x509write_crt_init( &crt );
    mbedtls_x509write_crt_set_md_alg( &crt, MBEDTLS_MD_SHA256 );
    mbedtls_mpi_init( &serial );
    mbedtls_ctr_drbg_init( &ctr_drbg );
#if defined(MBEDTLS_X509_CSR_PARSE_C)
    mbedtls_x509_csr_init( &csr );
#endif
    mbedtls_x509_crt_init( &issuer_crt );


    opt_cert.issuer_crt          = DFL_ISSUER_CRT;
    opt_cert.request_file        = DFL_REQUEST_FILE;
    opt_cert.issuer_key          = DFL_ISSUER_KEY;
    opt_cert.subject_pwd         = DFL_SUBJECT_PWD;
    opt_cert.issuer_pwd          = DFL_ISSUER_PWD;
    opt_cert.subject_name        = DFL_SUBJECT_NAME;
    opt_cert.issuer_name         = DFL_ISSUER_NAME;
    opt_cert.not_before          = DFL_NOT_BEFORE;
    opt_cert.not_after           = DFL_NOT_AFTER;
    opt_cert.serial              = DFL_SERIAL;
    opt_cert.selfsign            = DFL_SELFSIGN;
    opt_cert.is_ca               = DFL_IS_CA;
    opt_cert.max_pathlen         = DFL_MAX_PATHLEN;
    opt_cert.key_usage           = DFL_KEY_USAGE;
    opt_cert.ns_cert_type        = DFL_NS_CERT_TYPE;
    
    mbedtls_entropy_init( &entropy );
    if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
                               (const unsigned char *) pers,
                               strlen( pers ) ) ) != 0 )
    {
        mbedtls_printf( " failed\n  !  mbedtls_ctr_drbg_seed\n\n");// returned %d \n", ret);//, buf );
        goto exit;
    }


    if( ( ret = mbedtls_mpi_read_string( &serial, 10, opt_cert.serial ) ) != 0 )
    {
        mbedtls_printf( " failed\n  !  mbedtls_mpi_read_string\n\n");// returned -0x%02x - %s\n\n", -ret, buf );
        goto exit;
    }

    if( strlen( opt_cert.issuer_crt ) )
    {
        if( !mbedtls_pk_can_do( &issuer_crt.pk, MBEDTLS_PK_RSA ) ||
            mbedtls_mpi_cmp_mpi( &mbedtls_pk_rsa( issuer_crt.pk )->N,
                         &mbedtls_pk_rsa( *issuer_key )->N ) != 0 ||
            mbedtls_mpi_cmp_mpi( &mbedtls_pk_rsa( issuer_crt.pk )->E,
                         &mbedtls_pk_rsa( *issuer_key )->E ) != 0 )
        {
            mbedtls_printf( " failed\n  !  issuer_key does not match issuer certificate\n\n" );
            ret = -1;
            goto exit;
        }
    }


    if( opt_cert.selfsign )
    {
        opt_cert.subject_name = opt_cert.issuer_name;
    }

    mbedtls_x509write_crt_set_subject_key( &crt, issuer_key );
    mbedtls_x509write_crt_set_issuer_key( &crt, issuer_key );

    /*
     * 1.0. Check the names for validity
     */
    if( ( ret = mbedtls_x509write_crt_set_subject_name( &crt, opt_cert.subject_name ) ) != 0 )
    {
        mbedtls_printf( " failed\n  !  mbedtls_x509write_crt_set_subject_name\n\n");// returned -0x%02x - %s\n\n", -ret, buf );
        goto exit;
    }

    if( ( ret = mbedtls_x509write_crt_set_issuer_name( &crt, opt_cert.issuer_name ) ) != 0 )
    {
        mbedtls_printf( " failed\n  !  mbedtls_x509write_crt_set_issuer_name\n\n");// returned -0x%02x - %s\n\n", -ret, buf );
        goto exit;
    }

    ret = mbedtls_x509write_crt_set_serial( &crt, &serial );
    if( ret != 0 )
    {
        mbedtls_printf( " failed\n  !  mbedtls_x509write_crt_set_serial\n\n");// returned -0x%02x - %s\n\n", -ret, buf );
        goto exit;
    }

    ret = mbedtls_x509write_crt_set_validity( &crt, opt_cert.not_before, opt_cert.not_after );
    if( ret != 0 )
    {
        mbedtls_printf( " failed\n  !  mbedtls_x509write_crt_set_validity\n\n");// returned -0x%02x - %s\n\n", -ret, buf );
        goto exit;
    }

    ret = mbedtls_x509write_crt_set_basic_constraints( &crt, opt_cert.is_ca,
                                               opt_cert.max_pathlen );
    if( ret != 0 )
    {
        mbedtls_printf( " failed\n  !  x509write_crt_set_basic_contraints\n\n");// returned -0x%02x - %s\n\n", -ret, buf );
        goto exit;
    }


#if defined(MBEDTLS_SHA1_C)

    ret = mbedtls_x509write_crt_set_subject_key_identifier( &crt );
    if( ret != 0 )
    {
        mbedtls_printf( " failed\n  !  mbedtls_x509write_crt_set_subject_key_identifier\n\n");// returned -0x%02x - %s\n\n", -ret, buf );
        goto exit;
    }


    ret = mbedtls_x509write_crt_set_authority_key_identifier( &crt );
    if( ret != 0 )
    {
        mbedtls_printf( " failed\n  !  mbedtls_x509write_crt_set_authority_key_identifier returned\n\n");// -0x%02x - %s\n\n", -ret, buf );
        goto exit;
    }

#endif /* MBEDTLS_SHA1_C */

    if( opt_cert.key_usage )
    {
        mbedtls_printf( "  . Adding the Key Usage extension ..." );
        fflush( stdout );

        ret = mbedtls_x509write_crt_set_key_usage( &crt, opt_cert.key_usage );
        if( ret != 0 )
        {
            mbedtls_printf( " failed\n  !  mbedtls_x509write_crt_set_key_usage returned\n\n");// -0x%02x - %s\n\n", -ret, buf );
            goto exit;
        }

        mbedtls_printf( " ok\n" );
    }

    if( opt_cert.ns_cert_type )
    {
        mbedtls_printf( "  . Adding the NS Cert Type extension ..." );
        fflush( stdout );

        ret = mbedtls_x509write_crt_set_ns_cert_type( &crt, opt_cert.ns_cert_type );
        if( ret != 0 )
        {
            mbedtls_printf( " failed\n  !  mbedtls_x509write_crt_set_ns_cert_type returned \n\n");//-0x%02x - %s\n\n", -ret, buf );
            goto exit;
        }

        mbedtls_printf( " ok\n" );
    }

    /*
     * 1.2. Writing the request
     */
    mbedtls_printf( ".Writing the certificate..." );


		
    if( ( ret = write_certificate( &crt, mbedtls_ctr_drbg_random, &ctr_drbg, cert_buf ) ) < 0  ) //memory consuming as fuck 
    {
        mbedtls_printf( " failed\n  !  write_certifcate error code %d \n\n", ret );// -0x%02x - %s\n\n", -ret, buf );
        goto exit;
    }

    mbedtls_printf( " ok\n" );

exit:
    mbedtls_mpi_free( &serial );
    mbedtls_entropy_free( &entropy );
    mbedtls_x509write_crt_free( &crt );
    mbedtls_ctr_drbg_free( &ctr_drbg );
		
    return( ret );
}
