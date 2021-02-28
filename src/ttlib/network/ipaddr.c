/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       ipaddr.c
 * @ingroup    network
 * @author     tango
 * @date       2021-02-21 
 * @brief      ipaddr.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "ipaddr.h"
#include "../platform/native_memcpy.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static __tt_inline__ tt_bool_t tt_ipaddr_ipv6_to_ipv4(tt_ipv6_ref_t ipv6, tt_ipv4_ref_t ipv4)
{
    tt_trace_noimpl();
    return tt_false;
}

static __tt_inline__ tt_bool_t tt_ipaddr_ipv4_to_ipv6(tt_ipv4_ref_t ipv4, tt_ipv6_ref_t ipv6)
{
    tt_trace_noimpl();
    return tt_false;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

tt_void_t tt_ipaddr_clear(tt_ipaddr_ref_t ipaddr) 
{
    // check
    tt_assert_and_check_return(ipaddr);

    // done
    ipaddr->family  = TT_IPADDR_FAMILY_NONE;
    ipaddr->port    = 0;
    ipaddr->have_ip = 0;
}

tt_void_t tt_ipaddr_copy(tt_ipaddr_ref_t ipaddr, tt_ipaddr_ref_t copy) 
{
    // check
    tt_assert_and_check_return(ipaddr && copy);

    // done
    if(!copy->have_ip)
    {
        ipaddr->family = copy->family;
        ipaddr->port   = copy->port;
    }
    // attempt to copy ipv4
    else if(copy->family == TT_IPADDR_FAMILY_IPV4)
    {
        ipaddr->family  = TT_IPADDR_FAMILY_IPV4;
        ipaddr->port    = copy->port;
        ipaddr->have_ip = 1;
        ipaddr->u.ipv4  = copy->u.ipv4;
    }
    else
        *ipaddr = *copy;
}

tt_bool_t tt_ipaddr_is_empty(tt_ipaddr_ref_t ipaddr) 
{
    // check
    tt_assert_and_check_return_val(ipaddr, tt_false);
    
    // done
    return tt_ipaddr_ip_is_empty(ipaddr);
}

tt_bool_t tt_ipaddr_is_equal(tt_ipaddr_ref_t ipaddr, tt_ipaddr_ref_t other) 
{
    // check
    tt_assert_and_check_return_val(ipaddr && other, tt_false);

    // done
    if(ipaddr->port != other->port) return tt_false;

    // ip is equal?
    return tt_ipaddr_ip_is_equal(ipaddr, other); 
}
 
tt_char_t const* tt_ipaddr_cstr(tt_ipaddr_ref_t ipaddr, tt_char_t* data, tt_size_t maxn) 
{
    // check
    tt_assert_and_check_return_val(ipaddr && data && maxn >= TT_IPADDR_CSTR_MAXN, tt_null);

    // ipaddr is empty?
    if(tt_ipaddr_is_empty(ipaddr))
    {
        // make it
        tt_long_t size = snprintf(data, maxn - 1, "0.0.0.0:0");
        if(size >= 0) data[size] = '\0';
    }

    // ip is empty?
    else if(tt_ipaddr_ip_is_empty(ipaddr))
    {
        // make it
        tt_long_t size = snprintf(data, maxn - 1, "0.0.0.0:%u", ipaddr->port); 
        if(size >= 0) data[size] = '\0';
    }
    else if(ipaddr->family == TT_IPADDR_FAMILY_UNIX)
    {
        // check again
        tt_assert_and_check_return_val(maxn >= TT_UNIXADDR_CSTR_MAXN, tt_null);

        // make it
        tt_unixaddr_cstr(&ipaddr->u.unixaddr, data, maxn);
    }
    else
    {
        // make it
        tt_char_t buff[TT_IPADDR_CSTR_MAXN];
        tt_bool_t is_ipv6 = ipaddr->family == TT_IPADDR_FAMILY_IPV6;
        tt_long_t size = snprintf(data, maxn - 1, "%s%s%s:%u", is_ipv6? "[" : "", 
            tt_ipaddr_ip_cstr(ipaddr, buff, sizeof(buff)), is_ipv6? "[" : "", ipaddr->port);
    }

    return data;
}
 
tt_bool_t tt_ipaddr_cstr_set(tt_ipaddr_ref_t ipaddr, tt_char_t const* cstr, tt_int16_t port, tt_int8_t family) 
{
    // check
    tt_assert_and_check_return_val(ipaddr && cstr, tt_false);

    // save port
    tt_ipaddr_port_set(ipaddr, port);

    // save ip
    return tt_ipaddr_ip_cstr_set(ipaddr, cstr, family);
}
 
tt_void_t tt_ipaddr_ip_clear(tt_ipaddr_ref_t ipaddr) 
{
    // check
    tt_assert_and_check_return(ipaddr);

    // clear ip
    ipaddr->have_ip = 0;
}

tt_bool_t tt_ipaddr_ip_is_empty(tt_ipaddr_ref_t ipaddr) 
{
    // check
    tt_assert_and_check_return_val(ipaddr, tt_true);

    // ip empty?
    return !ipaddr->have_ip;
}
 
tt_bool_t tt_ipaddr_ip_is_any(tt_ipaddr_ref_t ipaddr) 
{
    // check
    tt_assert_and_check_return_val(ipaddr, tt_true);

    // check ip empty?
    tt_check_return_val(ipaddr->have_ip, tt_true);

    // done
    tt_bool_t is_any = tt_true;
    switch(ipaddr->family)
    {
    case TT_IPADDR_FAMILY_IPV4:
        is_any = tt_ipv4_is_any(&ipaddr->u.ipv4);
        break;
    case TT_IPADDR_FAMILY_IPV6:
        is_any = tt_ipv4_is_any(&ipaddr->u.ipv6);
        break;
    case TT_IPADDR_FAMILY_UNIX:
        is_any = tt_false;
        break;
    default:
        break;
    }

    return is_any;
}
 
tt_bool_t tt_ipaddr_ip_is_loopback(tt_ipaddr_ref_t ipaddr) 
{
    // check
    tt_assert_and_check_return_val(ipaddr, tt_true);

    // done
    tt_bool_t is_loopback = tt_false;
    switch(ipaddr->family)
    {
    case TT_IPADDR_FAMILY_IPV4:
        is_loopback = tt_ipv4_is_loopback(&ipaddr->u.ipv4);
        break;
    case TT_IPADDR_FAMILY_IPV6:
        is_loopback = tt_ipv6_is_loopback(&ipaddr->u.ipv6);
        break;
    case TT_IPADDR_FAMILY_UNIX:
        is_loopback = tt_false;
        break;
    default:
        break;
    }
    
    return is_loopback;
}
 
tt_bool_t tt_ipaddr_ip_is_equal(tt_ipaddr_ref_t ipaddr, tt_ipaddr_ref_t other) 
{
    // check
    tt_assert_and_check_return_val(ipaddr && other, tt_false);

    // done
    tt_bool_t ok = tt_false;
    do
    {   
         // both no ip
         if (!ipaddr->have_ip && !other->have_ip) ok = tt_true;
         // only one no ip
         else if (ipaddr->have_ip != other->have_ip) ok = tt_false;
         // both ipv4
         else if (ipaddr->family == TT_IPADDR_FAMILY_IPV4 && other->family == TT_IPADDR_FAMILY_IPV4)
         {
             ok =tt_ipv4_is_equal(ipaddr, other);
         }
         // both ipv6
         else if (ipaddr->family == TT_IPADDR_FAMILY_IPV6 && other->family == TT_IPADDR_FAMILY_IPV6)
         {
             ok = tt_ipv6_is_equal(ipaddr, other);
         }
         // both unix
         else if (ipaddr->family == TT_IPADDR_FAMILY_UNIX && other->family == TT_IPADDR_FAMILY_UNIX)
         {
             ok = tt_unixaddr_is_equal(ipaddr, other);
         }
         // only one unixaddr
         else if (ipaddr->family == TT_IPADDR_FAMILY_UNIX || other->family == TT_IPADDR_FAMILY_UNIX)
         {
             ok = tt_false;
         }
         // ipaddr is ipv6?
         else if (ipaddr->family == TT_IPADDR_FAMILY_IPV6)
         {
             tt_ipv4_t ipv4;
             ok = tt_ipaddr_ipv6_to_ipv4(&ipaddr->u.ipv6, &ipv4);
             if(ok) ok = tt_ipv4_is_equal(&ipv4, &other->u.ipv4);
         }
         // other is ipv6?
         else if (other->family == TT_IPADDR_FAMILY_IPV6)
         {
             tt_ipv4_t ipv4;
             ok = tt_ipaddr_ipv6_to_ipv4(&other->u.ipv6, &ipv4);
             if(ok) ok = tt_ipv4_is_equal(&ipaddr->u.ipv4, &ipv4);
         }
         else
         {
             // faile
             tt_trace_a("ipaddr_ip_csrt assert");
         }

    } while (0);
     
    return ok;
}

tt_char_t const* tt_ipaddr_ip_cstr(tt_ipaddr_ref_t ipaddr, tt_char_t* data, tt_size_t maxn) 
{
    // check
    tt_assert_and_check_return_val(ipaddr && data && maxn, tt_null);
    
    // done
    tt_char_t const* cstr = tt_null;
    switch (ipaddr->family)
    {
    case TT_IPADDR_FAMILY_IPV6:
        if(ipaddr->have_ip) cstr = tt_ipv6_cstr(&ipaddr->u.ipv6, data, maxn);
        else
        {
            // check
            tt_assert(maxn >= TT_IPV6_CSTR_MAXN);

            // make empty ipv6 
            tt_size_t size = snprintf(data, maxn - 1, "::");
            if (size >= 0) data[size] = '\0';

            // ok
            cstr = data;
        }
        break;

    case TT_IPADDR_FAMILY_IPV4:
        if(ipaddr->have_ip) cstr = tt_ipv4_cstr(&ipaddr->u.ipv4, data, maxn);
        else
        {
            // check
            tt_assert(maxn >= TT_IPV4_CSTR_MAXN);

            // make empty ipv4 
            tt_size_t size = snprintf(data, maxn - 1, "0.0.0.0");
            if (size >= 0) data[size] = '\0';

            // ok
            cstr = data;
        }
        break;

    case TT_IPADDR_FAMILY_UNIX:
        if(ipaddr->have_ip) cstr = tt_unixaddr_cstr(&ipaddr->u.unixaddr, data, maxn);
        else
        {
            // check
            tt_assert(maxn >= TT_UNIXADDR_CSTR_MAXN);

            // make empty unixaddr 
            tt_memset(data, 0, maxn);

            // ok
            cstr = data;
        }
        break;

    default:
        tt_trace_a("ipaddr_ip_csrt assert");
        break;
    }

    // ok
    return cstr;
}

tt_bool_t tt_ipaddr_ip_cstr_set(tt_ipaddr_ref_t ipaddr, tt_char_t const* cstr, tt_uint8_t family) 
{
    // check 
    tt_assert_and_check_return_val(ipaddr, tt_false);

    // no ip? clear it and return
    if (!cstr)
    {
        // check
        tt_assert(ipaddr);

        // clear ip
        ipaddr->have_ip = 0;
        ipaddr->family  = family;

        return tt_true;
    }

    // done
    tt_bool_t ok = tt_false;
    tt_ipaddr_t tmp;
    switch (ipaddr->family)
    {
    case TT_IPADDR_FAMILY_IPV6:
        // make ipv6
        ok = tt_ipv6_cstr_set(&tmp.u.ipv6, cstr);
        if(ok) tmp.family = family;
        break;
    
    case TT_IPADDR_FAMILY_IPV4:
        // make ipv4
        ok = tt_ipv4_cstr_set(&tmp.u.ipv4, cstr);
        if(ok) tmp.family = family;
        break;

    case TT_IPADDR_FAMILY_UNIX:
        // make unixaddr
        ok = tt_unixaddr_cstr_set(&tmp.u.unixaddr, cstr, tt_false);
        if(ok) tmp.family = family;
        break;
    
    default:
        break;
    }

    // save
    if(ok)
    {
        // get port
        tmp.port = ipaddr->port;

        // have ip
        tmp.have_ip = 1;

        // save
        tt_ipaddr_copy(ipaddr, &tmp);
    }

    return ok;
}

tt_void_t tt_ipaddr_ip_set(tt_ipaddr_ref_t ipaddr, tt_ipaddr_ref_t other) 
{
    // check
    tt_assert_and_check_return(ipaddr);

    // no ip, clear it, return fast
    if(!other)
    {
        // clear ip
        ipaddr->have_ip = 0;
        return;
    }

    // done
    switch (other->family)
    {
    case TT_IPADDR_FAMILY_IPV6:
        // save ipv6
        tt_ipaddr_ipv6_set(ipaddr, &other->u.ipv6);
        ipaddr->have_ip = 1;
        break;

    case TT_IPADDR_FAMILY_IPV4:
        // save ipv4
        tt_ipaddr_ipv4_set(ipaddr, &other->u.ipv4);
        ipaddr->have_ip = 1;
        break;
    
    case TT_IPADDR_FAMILY_UNIX:
        // save unixaddr
        tt_ipaddr_unix_set(ipaddr, &other->u.unixaddr);
        ipaddr->have_ip = 1;
        break;
    
    default:
        tt_trace_a("ipaddr ip set");
        break;
    }
}
 
tt_ipv4_ref_t tt_ipaddr_ipv4(tt_ipaddr_ref_t ipaddr) 
{
    // check
    tt_assert_and_check_return_val(ipaddr, tt_null);

    // no ip?
    tt_check_return_val(ipaddr->have_ip, tt_null);

    // done
    tt_ipv4_ref_t ipv4 = tt_null;
    switch (ipaddr->family)
    {
    case TT_IPADDR_FAMILY_IPV6:
        {
            tt_ipv4_t tmp;
            if (tt_ipaddr_ipv6_to_ipv4(&ipaddr->u.ipv6, &tmp))
            {
                ipaddr->family = TT_IPADDR_FAMILY_IPV4;
                ipaddr->u.ipv4 = tmp;
                ipv4 = &ipaddr->u.ipv4;
            }
        }
    break;

    case TT_IPADDR_FAMILY_IPV4:
        ipv4 = &ipaddr->u.ipv4;
        break;
    
    case TT_IPADDR_FAMILY_UNIX:
        break;

    default:
        tt_trace_a("ipaddr ipv4");
        break;
    }

    // ok?
    return ipv4;
}

tt_void_t tt_ipaddr_ipv4_set(tt_ipaddr_ref_t ipaddr, tt_ipv4_ref_t ipv4) 
{
    // check
    tt_assert_and_check_return(ipaddr);

    // on ipv4? clear it
    if(!ipv4)
    {
        ipaddr->have_ip = 0;
        return;
    }

    // done
    ipaddr->family  = TT_IPADDR_FAMILY_IPV4;
    ipaddr->u.ipv4  = *ipv4;
    ipaddr->have_ip = 1;
}

tt_ipv6_ref_t tt_ipaddr_ipv6(tt_ipaddr_ref_t ipaddr) 
{
    // check
    tt_assert_and_check_return_val(ipaddr, tt_null);

    // no ip?
    tt_check_return_val(ipaddr->have_ip, tt_null);

    // done
    tt_ipv6_ref_t ipv6 = tt_null;
    switch (ipaddr->family)
    {
    case TT_IPADDR_FAMILY_IPV4:
        {
            tt_ipv6_t tmp;
            if (tt_ipaddr_ipv4_to_ipv6(&ipaddr->u.ipv4, &tmp))
            {
                ipaddr->family = TT_IPADDR_FAMILY_IPV6;
                ipaddr->u.ipv6 = tmp;
                ipv6 = &ipaddr->u.ipv6;
            }
        }
        break;

    case TT_IPADDR_FAMILY_IPV6:
        ipv6 = &ipaddr->u.ipv6;
        break;
    
    case TT_IPADDR_FAMILY_UNIX:
        break;

    default:
        tt_trace_a("ipaddr ipv4");
        break;
    }

    // ok?
    return ipv6;
}

tt_void_t tt_ipaddr_ipv6_set(tt_ipaddr_ref_t ipaddr, tt_ipv6_ref_t ipv6) 
{
    // check
    tt_assert_and_check_return(ipaddr);

    // on ipv6? clear it
    if(!ipv6)
    {
        ipaddr->have_ip = 0;
        return;
    }

    // done
    ipaddr->family  = TT_IPADDR_FAMILY_IPV6;
    ipaddr->u.ipv6  = *ipv6;
    ipaddr->have_ip = 1;
}

tt_unixaddr_ref_t tt_ipaddr_unix(tt_ipaddr_ref_t ipaddr) 
{
    // check
    tt_assert_and_check_return_val(ipaddr, tt_null);

    // no ip?
    tt_check_return_val(ipaddr->have_ip, tt_null);

    // done
    if(ipaddr->family == TT_IPADDR_FAMILY_UNIX) return &ipaddr->u.unixaddr;

    return tt_null;
}
 
tt_void_t tt_ipaddr_unix_set(tt_ipaddr_ref_t ipaddr, tt_unixaddr_ref_t unixaddr) 
{
    // check
    tt_assert_and_check_return(ipaddr);

    // no ip
    if(!unixaddr)
    {
        ipaddr->have_ip = 0;
        return;
    }

    // done
    ipaddr->family     = TT_IPADDR_FAMILY_UNIX;
    ipaddr->u.unixaddr = *unixaddr;
    ipaddr->have_ip    = 1;
}
 
tt_bool_t tt_ipaddr_unix_set_cstr(tt_ipaddr_ref_t ipaddr, tt_char_t const* cstr, tt_bool_t is_abstract) 
{
    // check 
    tt_assert_and_check_return_val(ipaddr && cstr, tt_false);

    // make ipaddr
    tt_ipaddr_t tmp;
    if(!tt_unixaddr_cstr_set(&tmp.u.unixaddr, cstr, is_abstract))
    {
        return tt_false;
    }
    tmp.family = TT_IPADDR_FAMILY_UNIX;
    tmp.have_ip = 1;

    // save
    tt_ipaddr_copy(ipaddr, &tmp);

    // ok
    return tt_true;
}
 
tt_size_t tt_ipaddr_family(tt_ipaddr_ref_t ipaddr) 
{
    // check
    tt_assert_and_check_return_val(ipaddr, TT_IPADDR_FAMILY_NONE);

    return ipaddr->family;
}
 
tt_void_t tt_ipaddr_family_set(tt_ipaddr_ref_t ipaddr, tt_size_t family) 
{
    // check 
    tt_assert_and_check_return(ipaddr);

    // ipv4 --> ipv6
    if(ipaddr->family == TT_IPADDR_FAMILY_IPV4 && family == TT_IPADDR_FAMILY_IPV6)
    {
        tt_ipv6_t tmp;
        if(tt_ipaddr_ipv4_to_ipv6(ipaddr, &tmp))
        {
            ipaddr->family = TT_IPADDR_FAMILY_IPV6;
            ipaddr->u.ipv6 = tmp;
        }
    }
    // ipv6 --> ipv4
    else if(ipaddr->family == TT_IPADDR_FAMILY_IPV6 && family == TT_IPADDR_FAMILY_IPV4)
    {
        tt_ipv4_t tmp;
        if(tt_ipaddr_ipv6_to_ipv4(ipaddr, &tmp))
        {
            ipaddr->family = TT_IPADDR_FAMILY_IPV4;
            ipaddr->u.ipv4 = tmp;
        }
    }
    // unix can't be converted
    else if(ipaddr->family == TT_IPADDR_FAMILY_UNIX && family != TT_IPADDR_FAMILY_UNIX)
    {
        tt_trace_a("can not convert UNIX");
    }
    else ipaddr->family = family;

}
 
tt_size_t tt_ipaddr_port(tt_ipaddr_ref_t ipaddr) 
{
    // check 
    tt_assert_and_check_return_val(ipaddr, 0);

    // get port
    return ipaddr->port;
}

tt_void_t tt_ipaddr_port_set(tt_ipaddr_ref_t ipaddr, tt_size_t port) 
{
    // check 
    tt_assert_and_check_return(ipaddr);

    // set port
    ipaddr->port = port;
}

