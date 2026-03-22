#!/bin/gawk -f
#
# $Author: bucknerk $
# $Date: 2008/03/31 16:28:39 $
# $Id: create_index.awk,v 1.2 2008/03/31 16:28:39 bucknerk Exp $
#
# $Log: create_index.awk,v $
# Revision 1.2  2008/03/31 16:28:39  bucknerk
# tweaked
#
# Revision 1.1  2008/03/31 15:38:05  bucknerk
# new
#
# Revision 1.1  2007/10/17 15:36:22  bucknerk
# This is a new file
#
#
#
# This uses two input files to create the table in the index.html file that
# is used to the databse documentation.  The two files are
# "index_names.txt", created by the get_columns script, that has one
# database name per line. The other file is db.comments.sav. It is name this
# so that it won't be deleted when the .txt files are ALL removed.  Its
# format is a line beginning with a bang (!) then a tab, a database name, a
# tab and a comment.  The comment must not have any newlines in it but its
# length is not otherwise limited (except by gawk). These files can also
# contain lines that BEGIN with a splat (#) which are comments. These are
# ignored as are blank lines;
#########################################################################
BEGIN{ count=0;FS="\t";}
/^#/	{next;} #skip comments
/^$/	{next;} #skip empty lines
/^!/	{ # this handles lines in db_comments.sav
  	comment[indexes[$2]]=sprintf("%s",$NF);
	next;
	}
# remember that NO pattern matches EVERY line so this
# needs to be AFTER all the pattern lines
	{ #lines in the index_names.txt file
	  indexes[$1]=count;
	  names[count++]=sprintf("%s",$1);
	  next;
	}
END{
# This only prints the table and the ending of the index.html file.
# The index.sav file should have been copied to index.html to start this
# process.
  
  for(i=0;i<count;i++) {
    printf("<tr><td><a href=\"./%s_desc.html\">%s</a></td>\n",
	   names[i],names[i]);
    if(i in comment) 
      printf("  <td>%s </td></tr>\n",comment[i]);
    else 
      print("  <td>&nbsp;</td></tr>");
  }
  print "</table>";
  print "  <hr>";
  print "  You can actually see the contents of these databases ";
  print "by referring this page: <a href=";
  print "\"http://nucastrodata2.ornl.gov/phpd/cina_eval/cina_eval_sql/sql.php\">Contents</a>";
  print "</body></html>";
}
