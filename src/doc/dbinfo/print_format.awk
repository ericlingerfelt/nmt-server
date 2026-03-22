#!/bin/gawk -f
#####################################################################
# $Author: bucknerk $
# $Date: 2008/04/16 13:48:23 $
# $Id: print_format.awk,v 1.2 2008/04/16 13:48:23 bucknerk Exp $
#
# $Log: print_format.awk,v $
# Revision 1.2  2008/04/16 13:48:23  bucknerk
# revised
#
# Revision 1.1  2008/04/08 14:42:05  bucknerk
# new
#
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
	      while(count<9) {
		if(arr[count] ~ "^[[:space:]]*$") {
		  fields[dbcount,localcount,count-2] = sprintf("...");
		 } else {
		   fields[dbcount,localcount,count-2]=sprintf("%s",arr[count]);
		   sub("^[[:space:]]+","",fields[dbcount,localcount,count-2]);
		   sub("[[:space:]]+$","",fields[dbcount,localcount,count-2]);
		 }
		 count++;
	       }
	       if(arr[10] ~ "^[[:space:]]*$") {
		 fields[dbcount,localcount,7] = sprintf("...");
	       } else {
		 fields[dbcount,localcount,7]=sprintf("%s",arr[10]);
		 #if(fields[dbcount,localcount,6] ~ "Partition") 
		 #  printf("\n++%s++\n",fields[dbcount,localcount,6]);
	       }
	       sub("^[[:space:]]+","",fields[dbcount,localcount,7]);
	       sub("[[:space:]]+$","",fields[dbcount,localcount,7]);
	       localcount++;
	      }
END{
# how wide do I make the fields, I can try to use the max I just got
# or I can pick a value and wrap if needed.  let it go an see just what happens
  fieldnames[0]=sprintf("Field Name:");
  fieldnames[1]=sprintf("Type:");
  fieldnames[2]=sprintf("Collate:");
  fieldnames[3]=sprintf("Null:");
  fieldnames[4]=sprintf("Key:");
  fieldnames[5]=sprintf("Default:");
  fieldnames[6]=sprintf("Extra:");
  fieldnames[7]=sprintf("Comment:");
  nfields[dbcount]=localcount;
  dbcount++;
  #printf("Tables:\n");
  current_name="";
  for(i=0;i<dbcount;i++) {
    split(dbnames[i],hdr);
    if(current_name !~ hdr[2]) {
      current_name=sprintf("%s",hdr[2]);
      printf("\nDatabase: %s\n\tTables:\n",hdr[2]) >> "db_index.txt";
    }
    printf("\t\t%s\n",hdr[4]) >> "db_index.txt";
  }
  close("db_index.txt");
  current_name="";
  current_file="";
  for(i=0;i<dbcount;i++) {
    split(dbnames[i],hdr);
    if(current_file !~ hdr[2]) {
      if(length(current_file)> 1) {
	close(current_file);
      }
      current_file=sprintf("db_%s_tables.txt",hdr[2]);
      lpp=1;
      printf("Table: %s\n", hdr[4])> current_file;
    }
    else {
      printf("----------------------------------------\n")>> current_file;
      #printf("---------------------------------------\n")>> current_file;
      printf("Table: %s\n", hdr[4])>> current_file;
      lpp+=2;
    }
  #  printf("%-17s","Field Name")>> current_file;
  #  printf("%-34s","Type")>> current_file;
  #  printf("%-18s","Collate")>> current_file;
  #  printf("%-5s","Null")>> current_file;
  #  printf("%-10s","Key")>> current_file;
  #  printf("%-18s","Default")>> current_file;
  #  printf("%-15s","Extra")>> current_file;
  #  printf("%-27s\n","Comment")>> current_file;
  #  printf("--------------------------------")>> current_file;
  #  printf("--------------------------------")>> current_file;
  #  printf("--------------------------------\n")>> current_file;
    for(j=0;j<nfields[i];j++){
      #delete more;
      additional=0;
      for(k=0;k<8;k++) {
	delete mr;
	mc=0;
	if(length(fields[i,j,k]) > 40 ) {
	  mc=str_split(fields[i,j,k],39,mr);
	  printf("    %-12s%-40s\n",fieldnames[k],mr[1]) >> current_file;
	  #printf("mc=%d:\n",mc);
	  #for(d=0;d<mc;d++) {
	  #  printf("\t'%s'\n",mr[d]);
	  #}
	}
	else {
	  printf("    %-12s%-40s\n",fieldnames[k],fields[i,j,k]) >> current_file;
	}
	for(a=2;a<mc;a++) {
	  #sub("^[[:space:]]+","",mr[a]);
	  #sub("[[:space:]]+$","",mr[a]);
	  if(length(mr[a]) > 0) {
	    printf("%16s%-40s\n"," ",mr[a]) >> current_file;
	    lpp++;
	  }
	}
      }
    printf("\n") >> current_file;
    lpp+=9;
    if(lpp > 44) {
      while(lpp < 48) {
	printf("\n",lpp) >> current_file;
	lpp++;
      }
      lpp=0;
    }
    }
  }
  close(current_file);
}



function str_split(string,size,mr)
{
  total=length(string);
  #printf("\n*******CALLED with %s\n",string);
  if(total<=size) {
    mr[0]=1;
    mr[1]=string;
    return mr[0];
  }
  num=split(string,arr,"[^[:graph:]]+");
  if(num == 1) {
    #print("*********GOING TO TRY PUNCTUATION");
    num=split(string,arr,"[[:punct:]]|$");
    if(num==1) { 
      # no good places to split this.
      mr[0]=total / size;
      if((total % size) > 0) mr[0]++;
      for(x=0;x<mr[0];x++) {
	mr[x+1]=substr(string,x*size+1,size);
      }
      return mr[0];
    } 
    else {
      #print("++++++++++++++HANDLING WITH PUNCTUATION");
      mr[0]=0;
      x=0;
      fun_count=0;
      #printf("total length=%d\nnum pieces=%d\n",total,num);
      #for(y=0;y<num;y++) {
#	printf("%2d: %2d: %s\n",y,length(arr[y]),arr[y]);
#      }
      newstr=sprintf("%s",string);
      x=0;
      mr[0]++;
      this=0;
      hold="";
      while(x < num) {
	pos=match(newstr,"[[:punct:]]",nrarr);
	#means I am at the end of the possible divisions
	# might still be screwed up but...
	if(pos>0) {
	  tmp=substr(newstr,1,pos);
	  #printf("1:%s\n",tmp);
	  if(this==0) {
	    #printf("1.1:%s\n",tmp);
	    hold=sprintf("%s",tmp);
	    this=length(hold);
	  }
	  else {
	    if((length(tmp) + this ) > size) {
	      #printf("1.2:%s\n",tmp);
	      mr[mr[0]]=sprintf("%s",hold);
	      mr[0]++;
	      hold=sprintf("%s",tmp);
	    }
	    else {
	      #printf("1.3:%s\n",tmp);
	      hold=sprintf("%s%s",hold,tmp);
	    }
	    this=length(hold);
	  }
	}
	else {
	  #this=0;
	  if(length(newstr) > 0) {
	    if(this > 0) {
	      #printf("2:%s\n",newstr);
	      if((this +length(newstr)) > size) {
		mr[mr[0]]=sprintf("%s",hold);
		mr[0]++;
		mr[mr[0]]=sprintf("%s",newstr);
		break;
	      }
	      else {
		mr[mr[0]]=sprintf("%s%s",hold,newstr);
	      }
	    }
	    else {
	      #printf("3:%s\n",newstr);
	      mr[mr[0]]=sprintf("%s",newstr);
	    }
	  }
	break;
	}
	newstr=substr(newstr,pos+1);
	x++;
      }
      #printf("4:%d, %s ::%s\n",this,hold,newstr);
      if(this>0) {
	mr[mr[0]]=sprintf("%s",hold);
	mr[0]++;
      }
    }
    return mr[0];
  } # had to split on punctuation because no spaces;
  else { # can split on spaces and neatly concatenate it maybe
    #print("+++++++++++++++split on spaces");
    mr[0]=1;
    this=0;
    buf="";
    for(x=0;x<=num;x++) {
      #printf("\t%d(%d): %s\n",x,this,arr[x]);
      if((this + length(arr[x])) > size) {
	sub("^[[:space:]]+","",buf);
	sub("[[:space:]]+$","",buf);
	mr[mr[0]]=sprintf("%s",buf);
	mr[0]++;
	this=0;
	buf=sprintf("%s",arr[x]);
      }
      else {
	buf=buf " " arr[x];
      }
      this = length(buf);
    }
    if(this > 0) {
      mr[mr[0]]=sprintf("%s",buf);
      mr[0]++;
      #printf("END = '%s'\n",mr[mr[0]]);
    }
    return mr[0];
  }
} #end of function str_split



