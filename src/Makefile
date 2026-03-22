###########################################################################
#
# $Author: bucknerk $
# $Date: 2008/04/01 19:58:17 $
# $Id: Makefile,v 1.1 2008/04/01 19:58:17 bucknerk Exp $
#
# $Log: Makefile,v $
# Revision 1.1  2008/04/01 19:58:17  bucknerk
# new
#
# Revision 1.7  2008/03/05 14:14:19  bucknerk
# added portaldoc
#
# Revision 1.6  2007/12/05 20:08:00  bucknerk
# added maindoc target
#
# Revision 1.5  2007/11/09 16:16:27  bucknerk
# added doc waiting point
#
# Revision 1.4  2007/10/15 18:08:45  bucknerk
# fixed typo
#
# Revision 1.3  2007/09/19 12:34:45  bucknerk
# Just expanded doc coverage
#
# Revision 1.2  2007/09/07 13:14:52  bucknerk
# added clean and cvs operation
#
# Revision 1.1  2007/09/06 13:41:27  bucknerk
# New makefile
#
#
#
# Makefile for the php version of the cina evaluation suite.  This code is
# mainly scripts. This makefile's purpose is to handle basic functions that
# otherwise involve more typing than it is worth. Yes I am lazy.
#
###########################################################################
DCC=/home/bucknerk/bin/mydoxygen
CC=gcc
DOC_DIR=./doc
DB_DIR=./doc/dbinfo
DOCCONF=mdoc.conf




.PHONY: usage doc clean 

usage:
	@echo 
	@echo "Usage: make [ doc | clean ]"
	@echo 
	

doc: $(DOC_DIR) $(DB_DIR)
	$(DCC) $(DOCCONF)
	chmod 0775 $(DOC_DIR)/mass_code
	chmod 0664 $(DOC_DIR)/mass_code/*
	cd $(DB_DIR) && $(MAKE)

#
#$(DOC_DIR):
#	cvs co $(DOC_DIR)
#
#$(DB_DIR):
#	cvs co $(DB_DIR)
#
clean:
	cd $(DOC_DIR)/dbinfo && $(MAKE) clean
	/bin/rm -f $(DOC_DIR)/mass_code/*

