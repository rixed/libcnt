/* This file is part of libcnt.
 *
 * Copyright (C) 2005 Cedric Cellier.
 *
 * Libcnt is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * Libcnt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MicroModel; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef NET_H_040712
#define NET_H_040712
#include <libcnt/sys/neti.h>	/* so that one can have a local defined net_address */

typedef struct s_net_address net_address;

net_address *net_address_construct(net_address *this, char *ip, unsigned port);
static inline void net_address_destruct(net_address *this) {}

#endif
// vi:ts=3:sw=3
