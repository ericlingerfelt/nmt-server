/*************************************************************************/
/*! \file
 * \verbatim
 * $Author: bucknerk $ 
 * $Date: 2008/04/29 20:00:34 $ 
 * $Id: m_admin.js,v 1.3 2008/04/29 20:00:34 bucknerk Exp $
 *
 * $Log: m_admin.js,v $
 * Revision 1.3  2008/04/29 20:00:34  bucknerk
 * minor fixes
 *
 * Revision 1.2  2008/04/22 14:22:18  bucknerk
 * fixed stuff
 *
 * Revision 1.1  2008/04/08 14:39:43  bucknerk
 * new
 *
 * Revision 1.4  2008/03/05 13:34:30  bucknerk
 * minor changes
 *
 * Revision 1.3  2007/09/19 12:39:42  bucknerk
 * Changed the registration validation to allow country to empty but not address.
 * A few minor tweeks on the other functions and added some documentation. Also
 * moved closeme() around just because.
 *
 * Revision 1.2  2007/09/07 13:12:18  bucknerk
 * site_admin.css
 *
 * Revision 1.1  2007/09/06 13:59:12  bucknerk
 * These are just neew files to work with the site_admin.php.
 *
 * \endverbatim
 *
 * These are some simple scripts that do input form validation. Basically
 * these check that all the required fields are not empty and that the
 * password does not contain 'bad' characters, ones that may cause problems
 * trying to type in somewhere.  Then we check the password length and make
 * sure that the two passwords are the same and if they are 'distort' the
 * password before we transmit it. It is not a great distortion but it does
 * work.  A big problem that we will have the the computation of the
 * encoding put into the database as Eric is running the SHA1 hash on his
 * end and I have not been able to duplicate it.
 *
 ************************************************************************/
/*************************************************************************/
/*!
 * This function is a very simple function that is only currently used
 * by the site_admin_reg.php file. Its purpose to close the browser window,
 * which is normally considered very rude. Why? Because when getting a
 * password from UCAMS, we open a new browser window. If whoever is running
 * the registration uses the "EXIT" botton on the bottom of the UCAMS
 * password screen, it comes back to registration page. It happens that
 * currently, UCAMS is using a <b>GET</b> request to return to that page. 
 * So I check to see if the site_admin_reg.php has been called with 
 * <b>GET</b> and if it has, use this little function to close the
 * window. See site_admin_reg.php for more information.
 *
 * \return Always <i>true</i>;
 *
 * \sideeffect Should close the current browser window, If using Firefox (at
 * least version 1.5.0.12) and you have the "Force links that open new windows
 * to: open in a new tab" preference selected, this will close the tab if
 * called.
 *
 ***************************************************************************/
function closeme()
{
  window.close();
  return true;
}


function adminHome()
{
  window.location="./mass_admin.html";
  return true;
}


/*************************************************************************/
/*!
 * This function is designed to work with the f_clientWidth(),
 * f_clientHeight() functions.  The reason for it that how/if you can get
 * the width and height of a window depends on the browser being used or the
 * phase of the moon and such factors are ridiculously non-standard. This
 * then filters the attempts to test for a valid value and returns the
 * result. It was cribbed from a web document that presented it as workable
 * but not necessarily perfect.
 *
 * \return An integer representing the width/height of the window or 0. The
 * value selected is, by priority the <i>window</i> value, the
 * <i>document.element</i> value or the <i>document.body</i> value
 *
 * \sideeffect None
 ***************************************************************************/
function f_filterResults(n_win, n_docel, n_body) {
  var n_result = n_win ? n_win : 0;
  if (n_docel && (!n_result || (n_result > n_docel)))
      n_result = n_docel;
  return n_body && (!n_result || (n_result > n_body)) ? n_body :
    n_result;
}

/*************************************************************************/
/*!
 * This function is designed to test the various methods that could get the
 * width of the window/document.  The 0 in the 'else' part of the ?
 * statements is so that if the next component does not exist, the result
 * will be 0 as opposed to garbage. Then this tests for a value for
 * <ul>
 * <li> window.innerWidth
 * <li> document.documentElement.clientWidth
 * <li> document.body.clientWidth
 * </ul>
 * and in general one of these is set, but not necessarily.
 *
 * \return An integer client width
 * \sideeffect None
 *
 ***************************************************************************/
function f_clientWidth() {
  return f_filterResults (
    window.innerWidth ? window.innerWidth : 0,
    document.documentElement ? 
    document.documentElement.clientWidth : 0,
    document.body ?  document.body.clientWidth : 0);
}

/*************************************************************************/
/*!
 * This function is designed to test the various methods that could get the
 * width of the window/document.  The 0 in the 'else' part of the ?
 * statements is so that if the next component does not exist, the result
 * will be 0 as opposed to garbage. Then this tests for a value for
 * <ul>
 * <li> window.innerHeight
 * <li> document.documentElement.clientHeight
 * <li> document.body.clientHeight
 * </ul>
 * and in general one of these is set, but not necessarily.
 *
 * \return An integer client height
 * \sideeffect None
 *
 ***************************************************************************/

function f_clientHeight() {
  return f_filterResults (
    window.innerHeight ? window.innerHeight : 0,
    document.documentElement ? 
    document.documentElement.clientHeight : 0, document.body ?
    document.body.clientHeight : 0);
}

/*************************************************************************/
/*!
 * This function uses the f_clientWidth() and/or f_clientHeight() 
 * functions and resizes the 3 images (identified by Id i1, i2, i3)
 * by changing their height. Now this has the effect of rescaling the entire
 * image.  The height is used because that is what worked best for this
 * particular instance.  This function is used in the 
 * &lt;body&gt; element with <i>onload</i> and <i>onresize</i>.
 *
 * \return Nothing
 * \sideeffect Resize the images.
 *
 ***************************************************************************/
function do_img_resize() {
  var mw=f_clientWidth();
  var mh=f_clientHeight();
  document.getElementById('i1') ?
  document.getElementById('i1').height=(mw*.20) : 0;
  document.getElementById('i2') ?
  document.getElementById('i2').height=(mw*.20) : 0;
  document.getElementById('i3') ?
  document.getElementById('i3').height=(mw*.20) : 0;
}

/*************************************************************************/
/*!
 * This function is used on form submission to validate the passwords when
 * registering a new user.  Makes sure they are at least 8 characters long, 
 * they do not contain values that could cause mysql/php problems and 
 * that the PW1 and PW2 values are the same.
 * <p>
 * Once that is done, checks to make sure that the rest of the user-filled 
 * fields are non-empty then uses an XOR operation to distort the password
 * before sending over https link.  This is not perfectly safe but should be
 * safe enough for this instance.  It has the advantage of making it much
 * more confusing if the data is intercepted. 
 * <p>
 * If any of the checks fail, an <i>alert</i> popup is called with the error
 * message before the function can return. Thing is I don't know that the
 * <i>alert</i> is blocking (the function execution) or non-blocking, not 
 * that it matters in this case. 
 *
 * \return true if passwords are the same and contain valid characters,
 * false otherwise. Also returns false if any of the fields are empty.
 *
 * \sideeffect "Distorted" password assigned to the ENCODE post variable,
 * name of the variable containing the <i>distortion</i> (one of
 * EMAIL,INSTITUTION,NAME, or DUMMY) assigned to PW1 and "NONE" assigned to
 * PW2.
 *
 ***************************************************************************/

function reg_checkscript() {
  var i=0;
  if(document.forms["register"].PW1.value.length < 8) {
     alert("Passwords must be at least 8 characters.");
     return false;
     }
  for(i=0;i<document.forms["register"].PW1.value.length;i++) {
    if(document.forms["register"].PW1.value[i] <= " " ||
      document.forms["register"].PW1.value[i] > "~" ||
      document.forms["register"].PW1.value[i] == "'" ||
      document.forms["register"].PW1.value[i] == "." ||
      document.forms["register"].PW1.value[i] == "`" ||
      document.forms["register"].PW1.value[i] == "," ||
      document.forms["register"].PW1.value[i] == '"')
      {
       alert("Password contains illegal character." +
       "allowed values are between ! and ~ and not including " +
       'comma, period, \', " and `');
       return false;
       }
     }
  if (document.forms["register"].PW1.value < 
  document.forms["register"].PW2.value || 
  document.forms["register"].PW1.value >
  document.forms["register"].PW2.value){
     alert("Passwords do not match, please reenter.");
     return false;
     }
  else {
    if(document.forms["register"].NAME.value.length == 0 ) {
      alert("NAME cannot be blamk");
      return false;
      }
    if(document.forms["register"].USERNAME.value.length < 4 ) {
      alert("USERNAME cannot be blamk or less than 4 characters");
      return false;
      }
    if(document.forms["register"].EMAIL.value.length == 0 ) {
      alert("EMAIL cannot be blamk");
      return false;
      }
    if(document.forms["register"].INSTITUTION.value.length == 0 ) {
      alert("INSTITUTION cannot be blamk");
      return false;
      }
    if(document.forms["register"].COUNTRY.value.length == 0 ) {
      alert("COUNTRY cannot be blamk");
      return false;
      }
    if(document.forms["register"].ADDRESS.value.length == 0 ) {
      alert("ADDRESS cannot be blamk");
      return false;
      }
    var str=new Array(document.forms["register"].PW1.value.length );
    var pw=document.forms["register"].PW1.value;
    var distort="tomtomthepipersson";
    var itsname="DUMMY";
    if(document.forms["register"].EMAIL.value.length >= pw.length){
      distort=document.forms["register"].EMAIL.value;
      itsname="EMAIL";
      }
    else if(document.forms["register"].NAME.value.length >= pw.length){
      distort=document.forms["register"].NAME.value;
      itsname="NAME";
      }
    else if(document.forms["register"].INSTITUTION.value.length 
		    >= pw.length){
      distort=document.forms["register"].INSTITUTION.value;
      itsname="INSTITUTION";
      }
    else {
      var dummy="";
      var time=new Date();
      var val=time.getSeconds() + time.getMinutes()*60 + 
	    time.getHours()*3600;
      val += (time.getDate() -1)*86400;
      for(i=0;i< pw.length;i++)
	dummy += String.fromCharCode((val + i*time.getSeconds())%127)
      distort=dummy;
      document.forms["register"].DUMMY.value=dummy;
      }

    for(i=0;i< pw.length; i++) {
      str[i] = pw[i].charCodeAt(0) ^ distort[i].charCodeAt(0);
      }
    document.forms["register"].ENCODE.value=str;
    document.forms["register"].PW1.value = itsname;
    document.forms["register"].PW2.value = "NONE";
    }
  return true;
}
/*************************************************************************/
/*!
 * This function is used on form submission to only "distort" the password.
 * This should not really be used but this makes an easy to check "My
 * password doesn't work" complaints and to make sure that we put the
 * password in correctly.
 * <p>
 * Uses an XOR operation to distort the password
 * before sending over https link.  This is not perfectly safe but should be
 * safe enough for this instance.  It has the advantage of making it much
 * more confusing if the data is intercepted. 
 * <p>
 *
 * \return true if password contains valid characters,
 * false otherwise.
 *
 * \sideeffect "Distorted" password assigned to the ENCODE post variable,
 * name of the variable containing the <i>distortion</i> and DUMMY assigned
 * to PW1. 
 *
 ***************************************************************************/

function val_checkscript() {
  var i=0;
  /*
   * not using this because some existing passwords are NOT
   * 8 characters long.
  if(document.forms["validate"].PW1.value.length < 8) {
     alert("Passwords must be at least 8 characters.");
     return false;
     }
   */
  for(i=0;i<document.forms["validate"].PW1.value.length;i++) {
    if(document.forms["validate"].PW1.value[i] <= " " ||
      document.forms["validate"].PW1.value[i] > "~" ||
      document.forms["validate"].PW1.value[i] == "'" ||
      document.forms["validate"].PW1.value[i] == "." ||
      document.forms["validate"].PW1.value[i] == "`" ||
      document.forms["validate"].PW1.value[i] == "," ||
      document.forms["validate"].PW1.value[i] == '"')
      {
       alert("Password contains illegal character." +
       "allowed values are between ! and ~ and not including " +
       'comma, period, \', " and `');
       return false;
       }
     }
  var str=new Array(document.forms["validate"].PW1.value.length );
  var pw=document.forms["validate"].PW1.value;
  var distort="";
  var time=new Date();
  var val=time.getSeconds() + time.getMinutes()*60 + 
      time.getHours()*3600;
  val += (time.getDate() -1)*86400;
  for(i=0;i< pw.length;i++)
    distort += String.fromCharCode((val + i*time.getSeconds())%127)
  document.forms["validate"].DUMMY.value=distort;
  for(i=0;i< pw.length; i++) {
    str[i] = pw[i].charCodeAt(0) ^ distort[i].charCodeAt(0);
    }
  document.forms["validate"].ENCODE.value=str;
  document.forms["validate"].PW1.value = "DUMMY";
  return true;
}

function val_resetscript() {
  var i=0;
  if(document.forms["reset"].PW1.value.length < 8) {
     alert("Passwords must be at least 8 characters.");
     return false;
     }
  for(i=0;i<document.forms["reset"].PW1.value.length;i++) {
    if(document.forms["reset"].PW1.value[i] <= " " ||
      document.forms["reset"].PW1.value[i] > "~" ||
      document.forms["reset"].PW1.value[i] == "'" ||
      document.forms["reset"].PW1.value[i] == "." ||
      document.forms["reset"].PW1.value[i] == "`" ||
      document.forms["reset"].PW1.value[i] == "," ||
      document.forms["reset"].PW1.value[i] == '"')
      {
       alert("Password contains illegal character." +
       "allowed values are between ! and ~ and not including " +
       'comma, period, \', " and `');
       return false;
       }
     }
  var str=new Array(document.forms["reset"].PW1.value.length );
  var pw=document.forms["reset"].PW1.value;
  var distort="";
  var time=new Date();
  var val=time.getSeconds() + time.getMinutes()*60 + 
      time.getHours()*3600;
  val += (time.getDate() -1)*86400;
  for(i=0;i< pw.length;i++)
    distort += String.fromCharCode((val + i*time.getSeconds())%127)
  document.forms["reset"].DUMMY.value=distort;
  for(i=0;i< pw.length; i++) {
    str[i] = pw[i].charCodeAt(0) ^ distort[i].charCodeAt(0);
    }
  document.forms["reset"].ENCODE.value=str;
  document.forms["reset"].PW1.value = "DUMMY";
  return true;
}

