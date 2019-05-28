#include <stdio.h>
#include "ipaddr.h"

#ifdef WITH_TESTS
#include <cstdint>
#include <array>
#include <type_traits>

using ip6_addr_t = std::array<uint8_t, 16>;

static constexpr bool equals(struct in6_addr const& in6, ip6_addr_t const& addr) 
{
    for ( size_t i = 0; i < sizeof(in6.s6_addr); i++ )
        if ( in6.s6_addr[i] != addr[i] )
            return false;

    return true;
}

static_assert(std::is_same<decltype("127.0.0.1"_ipaddr), struct in_addr>::value);
static_assert(std::is_same<decltype("::1"_ipaddr), struct in6_addr>::value);
static_assert(IPAddr::inet_addr("1.2.3.4") == IPAddr::details::htonl(0x01020304));
static_assert(IPAddr::inet_addr("127.0.0.1") == IPAddr::details::htonl(0x7F000001));
static_assert(IPAddr::inet_addr("0x7F.0.0.1") == IPAddr::inet_addr("127.0.0.1"));
static_assert(IPAddr::inet_addr("0xc0.0XA8.1") == IPAddr::inet_addr("192.168.0.1"));
static_assert(IPAddr::inet_addr("0x7f.1") == IPAddr::inet_addr("127.0.0.1"));
static_assert(IPAddr::inet_addr("0177.1") == IPAddr::inet_addr("127.0.0.1"));
static_assert(IPAddr::inet_addr("2130706433") == IPAddr::inet_addr("127.0.0.1"));
static_assert(IPAddr::inet_addr("017700000001") == IPAddr::inet_addr("127.0.0.1"));
static_assert(IPAddr::inet_addr("0x7f000001") == IPAddr::inet_addr("127.0.0.1"));
static_assert(IPAddr::is_valid_ip4addr("") == false);
static_assert(IPAddr::is_valid_ip4addr(".") == false);
static_assert(IPAddr::is_valid_ip4addr("0.") == false);
static_assert(IPAddr::is_valid_ip4addr(".0") == false);
static_assert(IPAddr::is_valid_ip4addr(" 127.0.0.1") == false);
static_assert(IPAddr::is_valid_ip4addr("127.0.0.1 ") == false);
static_assert(IPAddr::is_valid_ip4addr("0.0.0.0.0") == false);
static_assert(IPAddr::is_valid_ip4addr("256.0.0.1") == false);
static_assert(IPAddr::is_valid_ip4addr("0x1h.2.3.4") == false);
static_assert(IPAddr::is_valid_ip4addr("0x100000000") == false);
static_assert(equals(IPAddr::inet_pton<AF_INET6>("::"), ip6_addr_t { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));
static_assert(equals(IPAddr::inet_pton<AF_INET6>("::1"), ip6_addr_t { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }));
static_assert(equals(IPAddr::inet_pton<AF_INET6>("1::"), ip6_addr_t { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));
static_assert(equals(IPAddr::inet_pton<AF_INET6>("1234:5678::1"), ip6_addr_t { 0x12, 0x34, 0x56, 0x78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }));
static_assert(equals(IPAddr::inet_pton<AF_INET6>("0:0:0:0:0:0:0:0"), ip6_addr_t { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0 }));
static_assert(IPAddr::is_valid_ip6addr("") == false);
static_assert(IPAddr::is_valid_ip6addr("1234") == false);
static_assert(IPAddr::is_valid_ip6addr("12345::") == false);
static_assert(IPAddr::is_valid_ip6addr("::00000") == false);
static_assert(IPAddr::is_valid_ip6addr(":::") == false);
static_assert(IPAddr::is_valid_ip6addr("1:::1") == false);
static_assert(IPAddr::is_valid_ip6addr(" ::") == false);
static_assert(IPAddr::is_valid_ip6addr(":: ") == false);
static_assert(IPAddr::is_valid_ip6addr("xyz::") == false);
static_assert(IPAddr::is_valid_ip6addr("0:0:0:0:0:0:0:0:0") == false);
#endif

int main() 
{
    int s = socket(AF_INET, SOCK_STREAM, 0);

    const struct sockaddr_in dst {
        .sin_family = AF_INET,
        .sin_port   = 1234_ipport,
        .sin_addr   = "127.0.0.1"_ipaddr
    };

    connect(s, (struct sockaddr *)&dst, sizeof(dst));

    return 0;
}
