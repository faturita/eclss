<?xml version="1.0" encoding="UTF-8" ?>
<!-- $Id: eclss.xsl,v 1.7 2005/10/29 15:15:10 fatu Exp $ -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
   <xsl:output method="html"/>
   <xsl:param name="error" select="'none'"/>
   <xsl:param name="html_url" select="'html/'"/>
   <xsl:param name="media_url" select="'images/'"/>
   <xsl:param name="parameters" select="''"/>
   <xsl:template match="document">


      <html>
         <SCRIPT LANGUAGE="JavaScript" type="text/JavaScript">

            <![CDATA[


             <!--

             function Validate(frm) {
             if (frm.pwd.value.length < 1) {
             validatePrompt(frm.pwd, "Ingrese la clave");
             return false;
             }

             if (frm.usr.value.length==0) {
             validatePrompt(frm.usr, "Ingrese el usuario");
             return false;
             }


             var usuario = "";
             for (i=0; i<frm.usr.value.length; i++) {
             if (frm.usr.value.charAt(i) != "."){
             usuario = usuario + (frm.usr.value.charAt(i));
             }
             }
             frm.usr.value = usuario;

             for (i=0; i<frm.usr.value.length; i++){
             if ("0123456789".indexOf(frm.usr.value.charAt(i))==-1) {
             validatePrompt(frm.usr, "Debe ingresar un valor num&#233;rico en usuario");
             return false;
             }
             }

             return true;
             }

             function validatePrompt(Ctrl,PromptStr) {
             Ctrl.focus();
             Ctrl.select();
             alert(PromptStr);
             }

             function openNewWindow(URLtoOpen, windowName, windowFeatures) {
             newWindow=window.open(URLtoOpen, windowName, windowFeatures);
             }


             function MM_reloadPage(init) {  //reloads the window if Nav4 resized
             if (init==true) with (navigator) {
             if ((appName=="Netscape")&&(parseInt(appVersion)==4)) {
             document.MM_pgW=innerWidth;
             document.MM_pgH=innerHeight;
             onresize=MM_reloadPage;
             }
             }else if (innerWidth!=document.MM_pgW || innerHeight!=document.MM_pgH){
             location.reload();
             }
             MM_reloadPage(true);
             }
             function checkEnter(){
             if( window.event.keyCode == 13 ){
             document.frm.submit();
             }
             }

             function MM_swapImgRestore() { //v3.0
             var i, x, a = document.MM_sr;
             for (i=0; a && i < a.length&&(x = a[i]) && x.oSrc; i++) {
             x.src = x.oSrc;
             }
             }

             function MM_swapImage() { //v3.0
             var i, j = 0, x, a = MM_swapImage.arguments;
             document.MM_sr = new Array;
             for (i = 0; i < (a.length-2); i += 3) {
             if ((x=MM_findObj(a[i])) != null) {
             document.MM_sr[j++] = x;
             if (!x.oSrc) {
             x.oSrc = x.src;
             }
             x.src = a[i+2];
             }
             }
             }

             function MM_preloadImages() { //v3.0
             var d=document;
             if (d.images) {
             if (!d.MM_p)
             d.MM_p = new Array();
             var i, j = d. MM_p.length, a = MM_preloadImages.arguments;
             for (i = 0; i < a.length; i++) {
             if (a[i].indexOf("#") != 0) {
             d.MM_p[j] = new Image;
             d.MM_p[j++].src = a[i];
             }
             }
             }
             }
             function MM_findObj(n, d) { //v4.0
             var p,i,x;
             if (!d) {
             d = document;
             }
             if ((p = n.indexOf("?")) > 0&&parent.frames.length) {
             d = parent.frames[n.substring(p + 1)].document;
             n = n.substring(0, p);
             }
             if (!(x=d[n])&&d.all) {
             x=d.all[n];
             }
             for (i=0;!x&&i<d.forms.length;i++) {
             x=d.forms[i][n];
             }
             for (i = 0;! x && d.layers && i < d.layers.length; i++) {
             x = MM_findObj(n,d.layers[i].document);
             }
             if (!x && document.getElementById) {
             x = document.getElementById(n);
             }
             return x;
             }

             function reload() {
               window.location.reload();
             }

             setTimeout('reload()',40000);

//-->
            ]]>

         </SCRIPT>

         <head>
            <title>ECLSS - Environment Control and Life Support System</title>
            <meta https-equiv="Content-Type" content="text/html; charset=iso-8859-1"/>
            <link rel="alternate" href="/sumariorss.xml" type="application/rss+xml" title="Application Status"/>
            <link rel="shortcut icon" href="/favicon.ico" type="image/x-icon"/>
            <style type="text/css">
               <!--
                  .highlight {color: #eeeef8; background-color: #737b9c; }
                  th { text-align: left; font-family: tahoma; font-size: 11px; }
                  td { text-align: left; font-family: tahoma; font-size: 11px; }
                  body { font-family: tahoma; font-size: 11px; }
                  input { font-family: tahoma; font-size: 11px; }
                  select { font-family: tahoma; font-size: 11px; }
               -->
            </style>

         </head>


         <body onKeyPress="checkEnter();">

            <table border="1" >
	       <h3 align="center"><em><b>Web Application Monitor (<a href="help">Ayuda</a>)</b></em></h3>
               <thead>
                  <em><b>Connection Status</b></em>
               </thead>
               <tbody>
                  <xsl:for-each select="//document/connection">
                     <xsl:variable name="backgroundcolor"><xsl:choose><xsl:when test="@status!='Ok'">#fb1106</xsl:when><xsl:otherwise>#eeeef8</xsl:otherwise></xsl:choose></xsl:variable>
                     <tr bgcolor="{$backgroundcolor}">
                        <td>
                           <xsl:value-of select="@name" />
                        </td>
                        <td>
                           Status: <xsl:value-of select="@status" />
                        </td>
                        <td>
                           <xsl:if test="@status!='Ok'">
                              Sent Message: <xsl:value-of select="@chkmsg" />
                           </xsl:if>
                        </td>
                     </tr>
                  </xsl:for-each>

               </tbody>
            </table>

            <table width="100%" cellspacing="1">
               <thead>
                  <tr color="eeeef0" bgcolor="#737b9c">
                     <td>App</td>
                     <td>Status</td>
                     <td colspan="2">Link</td>
                     <td>Response Time</td>
                  </tr>
               </thead>
               <tfoot>
                  <tr>
                     <td colspan="6">
                     </td>
                  </tr>
               </tfoot>
               <tbody>
                  <caption>
                     <em><b>App Web</b></em>
                  </caption>
                  <xsl:for-each select="//document/application">
                     <xsl:sort select="@status"/>
                     <xsl:variable name="bgcolor">
                        <xsl:choose>
                           <xsl:when test="@status!='Online'">#fb1106</xsl:when>
                           <xsl:when test="position() mod 2=0">#eeeee0</xsl:when>
                           <xsl:otherwise>#eeeef8</xsl:otherwise>
                        </xsl:choose>
                     </xsl:variable>

                     <xsl:variable name="rowspan" select="count(url)+1"/>
                     <tr bgcolor="{$bgcolor}">
                        <td class="highlight" rowspan="{$rowspan}">
                           <em>
			      <xsl:variable name="helpurl" select="@helpurl" />
			      <xsl:if test="@helpurl" >
                                <a href="{$helpurl}" ><xsl:value-of select="@name"/></a>
			      </xsl:if>
      
			      <xsl:if test="not(@helpurl)" >
                                <xsl:value-of select="@name"/>
			      </xsl:if>
                           </em>
                        </td>
                        <td rowspan="{$rowspan}">
                           <xsl:value-of select="@status"/><br/>
                           <xsl:choose><xsl:when test="@status!='Online'">(Downtime:</xsl:when><xsl:otherwise>(Uptime:</xsl:otherwise></xsl:choose>
                           <xsl:value-of select="@timelife"/>s)
                        </td>
                     </tr>
                     <!-- td -->
                     <!-- table width="100%" border="1" cellspacing="1" -->
                     <xsl:for-each select="url">
                        <tr bgcolor="{$bgcolor}">
                           <td width="40">
                              <xsl:value-of select="@detail"/>
                           </td>
                           <td width="200">
                              <xsl:variable name="url" select="@url"/>
                              <xsl:variable name="status" select="@status"/>
                              <xsl:variable name="httpretcode" select="@httpretcode"/>
			      <xsl:variable name="urlstatus" select="@urlstatus"/>
                              <a title="{$status} {$urlstatus} {$httpretcode}" href="{$url}">
                                 <xsl:value-of select="$url"/>
                              </a>
                           </td>
                           <xsl:variable name="latency" select="@latency"/>
                           <xsl:variable name="timeout"><xsl:choose><xsl:when test="@latency &gt; 15000">red</xsl:when><xsl:when test="@latency &gt; 5000 and @latency &lt; 15001">yellow</xsl:when><xsl:otherwise>green</xsl:otherwise></xsl:choose></xsl:variable>
                           <td align="center" bgcolor="{$timeout}" width="20">
                              <xsl:value-of select="@latency"/> ms
                           </td>
                        </tr>
                     </xsl:for-each>
                     <!-- /table -->
                     <!-- /td -->
                     <!-- /tr -->
                  </xsl:for-each>
               </tbody>
            </table>
            <hr/>
            <i>Uptime <xsl:value-of select="//document/display/uptime"/>.</i>

         </body>
      </html>
   </xsl:template>
</xsl:stylesheet>
