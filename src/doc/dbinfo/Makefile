##########################################################################
#
# $Author: bucknerk $
# $Id: Makefile,v 1.1 2008/03/31 15:38:05 bucknerk Exp $
#
# $Log: Makefile,v $
# Revision 1.1  2008/03/31 15:38:05  bucknerk
# new
#
#
#################################################
# borrowed from cina_eval
#
# Revision 1.3  2007/11/09 16:25:02  bucknerk
# changed file perms on line 26
#
# Revision 1.2  2007/10/17 15:30:27  bucknerk
# Removed the renaming of index.html to index.sav.  Now .sav is used to
# generate .html BUT is not the etire file.
#
# Revision 1.1.1.1  2007/09/06 13:34:20  bucknerk
# Database documentation
#
#
#
# Simple file to run a script that creates some html files based on the
# MySql databases used for cina and bbn.  Makes it easier sometimes to be
# able to see all the information in a reasonably formatted table.
##########################################################################



all:
	./get_columns
	chmod 0664 *.html


clean:
	/bin/rm -f *.html
