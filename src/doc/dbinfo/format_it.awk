#!/bin/gawk -f
#####################################################################
# $Author: bucknerk $
# $Date: 2008/03/31 15:38:05 $
# $Id: format_it.awk,v 1.1 2008/03/31 15:38:05 bucknerk Exp $
#
# $Log: format_it.awk,v $
# Revision 1.1  2008/03/31 15:38:05  bucknerk
# new
#
# Revision 1.2  2007/10/17 15:31:07  bucknerk
# Changed how the lines are parsed and added the Comment column to the output
#
# Revision 1.1.1.1  2007/09/06 13:34:19  bucknerk
# Database documentation
#
#
#
#
# This is the script that formats the output from the mysql database show
# operations and creates the .html files.  It is fairly simple and is run on
# each file independently.  Remember that BEGIN is only done before the
# first input file is accessed and the END is done after the last file is
# closed.
#####################################################################
BEGIN{started=0;
dbcount=0;
print("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"");
print("     \"http://www.w3.org/TR/html4/loose.dtd\">");
print("<html>");
print("  <head>");
print("\t<title>");
print("Description of the database tables");
print("\t</title>");
print("  </head>");
}
/^Database/ 	{
              if(started){
		nfields[dbcount]=localcount;
		dbcount++;
	      }
	      else started=1;
	      dbnames[dbcount]=sprintf("%s",$0); 
	      mydbname=sprintf("%s",$2);
	      localcount=0;
	       next;
	      }
/Field/		{
	       next;
	      }
/\+---/		{next;}
/^$/		{next;}
/^#####/	{
	       next;
	      }
	      {
	      split($0,arr,"|");
	      count=2;
	      while(count<8) {
		if(arr[count] ~ "^[[:space:]]*$") {
		  fields[dbcount,localcount,count-2] = sprintf("&nbsp;");
		 } else {
		   fields[dbcount,localcount,count-2]=sprintf("%s",arr[count]);
		 }
		 count++;
	       }
	       if(arr[10] ~ "^[[:space:]]*$") {
		 fields[dbcount,localcount,6] = sprintf("&nbsp;");
	       } else {
		 fields[dbcount,localcount,6]=sprintf("%s",arr[10]);
	       }
	       localcount++;
	      }
END{
print("  <body>");
print("<img src=\"mysql_logo.jpg\">");
print("<br><hr>");
printf("<h1>Description of the %s database tables </h1>",toupper(mydbname));
print("<hr><hr>");
print("<a name=\"index\">Table Index</a>");
print("<ul>");
  nfields[dbcount]=localcount;
  dbcount++;
for(i=0;i<dbcount;i++) {
  split(dbnames[i],hdr);
  printf("<li><a href=\"#%s\">%s</a>",hdr[4],hdr[4]);
}
print("</ul>");
for(i=0;i<dbcount;i++) {
  split(dbnames[i],hdr);
  printf("<p><a name=%s>Database: <b>%s</b> Table: <b>%s</b></a>\n",
	 hdr[4],hdr[2],hdr[4]);
  print("<br>Up to <a href=\"#index\">Index</a><p>");
  print("<table width=\"99%\" border="1">");
  print("<tr><th>Field</th>");
  print("<th>Type</th>");
  print("<th>Collate</th>");
  print("<th>Null</th>");
  print("<th>Key</th>");
  print("<th>Default</th>");
  print("<th>Comment</th>");
  for(j=0;j<nfields[i];j++){
    print("<tr>");
    for(k=0;k<7;k++) {
      printf("<td>%s</td>\n",fields[i,j,k]);
    }
    print("</tr>");
  }
  print("</table>");
}
  print("  </body>");
  print("</html>");

  
  
  }
