// -*- c-basic-offset: 4 -*-
/*
 * ip6 handler kludge by Marko Zec
 *
 * originates from iproutetable.{cc,hh} by Benjie Chen, Eddie Kohler
 *
 * Copyright (c) 2001 Massachusetts Institute of Technology
 * Copyright (c) 2002 International Computer Science Institute
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, subject to the conditions listed in the Click LICENSE
 * file. These conditions include: you must preserve this copyright
 * notice, and you cannot mention the copyright holders in advertising
 * related to the Software without their permission.  The Software is
 * provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This notice is a
 * summary of the Click LICENSE file; the license in that file is
 * legally binding.
 */

#include <click/config.h>
#include <click/ip6address.hh>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include "ip6routetable.hh"
#include <stdlib.h>
CLICK_DECLS

void *
IP6RouteTable::cast(const char *name)
{
    if (strcmp(name, "IPRouteTable") == 0)
	return (void *)this;
    else
	return Element::cast(name);
}

int
IP6RouteTable::add_route(IP6Address, IP6Address, IP6Address,
			 int, ErrorHandler *errh)
{
    // by default, cannot add routes
    return errh->error("cannot add routes to this routing table");
}

int
IP6RouteTable::remove_route(IP6Address, IP6Address, ErrorHandler *errh)
{
    // by default, cannot remove routes
    return errh->error("cannot delete routes from this routing table");
}

String
IP6RouteTable::dump_routes()
{
    return String();
}

int
IP6RouteTable::add_route_handler(const String &conf, Element *e, void *, ErrorHandler *errh)
{
    IP6RouteTable *r = static_cast<IP6RouteTable *>(e);

    Vector<String> words;
    cp_spacevec(conf, words);

    IP6Address dst, mask, gw;
    int port, ok;

    if (words.size() == 2)
        ok = cp_va_kparse(words, r, errh,
			  "PREFIX", cpkP+cpkM, cpIP6AddressOrPrefix, &dst, &mask,
			  "PORT", cpkP+cpkM, cpInteger, &port, cpEnd);
    else
        ok = cp_va_kparse(words, r, errh,
			  "PREFIX", cpkP+cpkM, cpIP6AddressOrPrefix, &dst, &mask,
			  "GATEWAY", cpkP+cpkM, cpIP6Address, &gw,
			  "PORT", cpkP+cpkM, cpInteger, &port, cpEnd);

    if (ok >= 0 && (port < 0 || port >= r->noutputs()))
        ok = errh->error("output port out of range");
    if (ok >= 0)
        ok = r->add_route(dst, mask, gw, port, errh);
    return ok;
}

int
IP6RouteTable::remove_route_handler(const String &conf, Element *e, void *, ErrorHandler *errh)
{
    IP6RouteTable *r = static_cast<IP6RouteTable *>(e);

    Vector<String> words;
    cp_spacevec(conf, words);

    IP6Address a, mask;
    int ok = 0;

    ok = cp_va_kparse(words, r, errh,
		      "PREFIX", cpkP+cpkM, cpIP6AddressOrPrefix, &a, &mask,
		      cpEnd);

    if (ok >= 0)
	ok = r->remove_route(a, mask, errh);
    return ok;
}

int
IP6RouteTable::ctrl_handler(const String &conf_in, Element *e, void *thunk, ErrorHandler *errh)
{
    String conf = conf_in;
    String first_word = cp_shift_spacevec(conf);
    if (first_word == "add")
	return add_route_handler(conf, e, thunk, errh);
    else if (first_word == "remove")
	return remove_route_handler(conf, e, thunk, errh);
    else
	return errh->error("bad command, should be `add' or `remove'");
}

String
IP6RouteTable::table_handler(Element *e, void *)
{
    IP6RouteTable *r = static_cast<IP6RouteTable*>(e);
    return r->dump_routes();
}

int
IP6RouteTable::generate_routes_handler(const String &conf, Element *e, void *, ErrorHandler *errh)
{
    IP6RouteTable* table = dynamic_cast<IP6RouteTable*>(e);
    assert(table);

    String conf_copy = conf;

    String count_str = cp_shift_spacevec(conf_copy);
    int count;
    if (!cp_integer(count_str, &count))
        return errh->error("invalid entry count: ", count_str.c_str());

    String port_str = cp_shift_spacevec(conf_copy);
    int port;
    if (!cp_integer(port_str, &port))
        return errh->error("invalid port: ", port_str.c_str());

    IP6Address gw("::0");

    unsigned short xsubi[3];
    xsubi[0] = 1;
    xsubi[1] = 2;
    xsubi[2] = 3;

    for (int i = 0; i < count - 1; i++)
    {
        struct click_in6_addr addr;
        addr.in6_u.u6_addr32[0] = static_cast<uint32_t>(nrand48(xsubi));
        addr.in6_u.u6_addr32[1] = static_cast<uint32_t>(nrand48(xsubi));
        addr.in6_u.u6_addr32[2] = static_cast<uint32_t>(nrand48(xsubi));
        addr.in6_u.u6_addr32[3] = static_cast<uint32_t>(nrand48(xsubi));
        int prefix_len = 8 + (addr.in6_u.u6_addr8[15] % 17);    // 8--24
        IP6Address mask = IP6Address::make_prefix(prefix_len);

        if (table->add_route(IP6Address(addr) & mask, mask, gw, port, errh))
            return -1;
    }

    if (table->add_route(IP6Address("::0"), IP6Address("::0"), gw, port, errh))
        return -1;

    click_chatter("generated %d entries", count);
    return 0;
}

CLICK_ENDDECLS
ELEMENT_PROVIDES(IP6RouteTable)
