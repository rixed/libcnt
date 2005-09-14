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
/* 
 * The C function sthat compose the av_list library
 *
 * 20/12/2002 - rixed@free.fr
 */

#include <stdlib.h>
#include <stddef.h>
#include "av_list.h"

av_list *av_new(void *function, size_t ret_size) {
	const unsigned nb_max_parameters = 10;	// number of parameters by default
	av_list *this = malloc(sizeof(av_list)+sizeof(struct av_parameter)*nb_max_parameters);
	if (NULL == this) return NULL;
	this->nb_parameters = 0;
	this->nb_max_parameters = nb_max_parameters;
	this->function = function;
	this->ret_size = ret_size;
	if (ret_size) {
		size_t s = ret_size;
		if (s<sizeof(int)) s=sizeof(int);
		this->ret_value = malloc(s);
		if (NULL == this->ret_value) {
			free(this);
			return NULL;
		}
	}
	return this;
}

int av_arg(av_list *this, void *parameter, size_t size) {
	if (NULL == this || NULL == parameter || 0 == size) return 0;
	if (this->nb_parameters >= this->nb_max_parameters) {	// resize
		unsigned new_nb_max_parameters = 1+ this->nb_max_parameters * 2;	// 1+ in case nb_max_parameters==0
		av_list *new = realloc(this, sizeof(av_list)+sizeof(struct av_parameter)*new_nb_max_parameters);
		if (NULL == new) return 0;
		this = new;
		this->nb_max_parameters = new_nb_max_parameters;
	}
	this->parameters[this->nb_parameters].parameter = parameter;
	this->parameters[this->nb_parameters].size = size;
	this->nb_parameters++;
	return 1;
}

void av_delete(av_list *this) {
	if (this->ret_size) free(this->ret_value);
	free(this);
}

// vi:ts=3:sw=3
