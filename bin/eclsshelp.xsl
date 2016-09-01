<?xml version="1.0" encoding="ISO-8859-1" ?>
<!-- $Id: eclsshelp.xsl,v 1.2 2005/10/29 15:15:10 fatu Exp $ -->
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

             //setTimeout('reload()',40000);

//-->
            ]]>

         </SCRIPT>

         <head>
            <title>ECLSS - Environment Control and Life Support System</title>
            <meta https-equiv="Content-Type" content="text/html; charset=iso-8859-1"/>
            <link rel="alternate" href="/sumariorss.xml" type="application/rss+xml" title="Status Aplicaciones"/>
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
               <thead>
                  <em><b>Ayuda (Sorry this page is in Spanish, Translation required)</b></em>
               </thead>
	       <h2>
	       Sistema de monitoreo de aplicaciones y servicios Web.
	       </h2>
	       <p>
	       Caracteristicas
	       </p>
		   <ul>
		     <li>Cada 50 s el sistema chequea el status de todas las conexiones configuradas.  Ante cualquier error en cualquiera de las URL que componen a una aplicacion se incrementa un contador.  Cuando este contador alcanza 3 se informa el primer email de aviso a las direcciones configuradas.  Posteriormente cada 5 hits que la aplicacion continue bajo se informa nuevamente (todo esto es configurable)</li>
		     <li>El chequeo es en las siguientes capas y en el sgte. orden:
		       <ul>
		         <li>IP:  Chequea acceso a la IP que obtiene al resolver el nombre.  La resolucion del nombre se realiza mediante mecanismo estandar en el equipo que corra la aplicacion</li>
			 <li>TCP: Intenta establecer una conexion al puerto correspondiente en base al URI definido para cada url de cada aplicacion.</li>
			 <li>SSL: En el caso que corresponda intenta establecer una sesion SSL con el servidor. El tiempo de espera definido para intentar establecer la sesion SSL es de 10 s.</li>
			 <li>HTTP: Envia un requerimiento HTTP y espera el resultado hasta un maximo de 50 s. Los codigos de respuesta HTTP que se aceptan como CORRECTOS son 200 (OK), 302(Redirect) y 404(Requiere autenticacion)</li> 
			 <li>Capa aplicacion: Para las conexiones que lo permitan heuristicamente analiza la respuesta enviada por el sistema buscando "marcas de agua" prefijas que impliquen un error a nivel aplicacion (por ejemplo busca la frase "Se ha producido un error en la aplicacion" o "Estamos mejorando el servicio".</li>
		       </ul>
		     </li>
                     <li>Algunas conexiones, para no sobrecargar los servicios con el muestreo, tienen intervalos de chequeo mayores.</li>
		     <li>Cada 45 s se actualiza el html reflejando el status de cada conexion que el sistema guarda en memoria. </li>
		   </ul>
               
        	<hr/>
        	<i></i>
	    </table>
         </body>
      </html>
   </xsl:template>
</xsl:stylesheet>
