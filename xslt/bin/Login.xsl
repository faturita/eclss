<?xml version="1.0" encoding="UTF-8"?>
<!-- $Id: Login.xsl,v 1.1.1.1 2005/02/18 19:19:00 vexrarod Exp $ -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="html"/>
    <!-- xsl:include href="...directorioXSL.../Contacto.xsl" />
    <xsl:include href="...directorioXSL.../Footer.xsl" / -->
    <xsl:param name="error" select="'none'"/>
    <xsl:param name="html_url" select="'html/'"/>
    <xsl:param name="media_url" select="'images/'"/>
    <xsl:param name="olvido_url" select="'pid/app/GetBienvenida.html'"/>
    <xsl:param name="parameters" select="''"/>
    <xsl:template match="document">

        <xsl:variable name="link1">
            <xsl:value-of  select="seccionServicios1/link"/>
        </xsl:variable>

        <xsl:variable name="link2">
            <xsl:value-of  select="seccionServicios2/link"/>
        </xsl:variable>

        <xsl:variable name="link3">
            <xsl:value-of  select="seccionServicios3/link"/>
        </xsl:variable>

        <xsl:variable name="link4_1">
            <xsl:value-of  select="seccionServicios4/link"/>
        </xsl:variable>

        <xsl:variable name="link4_2">
            <xsl:value-of  select="seccionServicios4/link2"/>
        </xsl:variable>

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

                //Le saco los puntos si los tiene
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

//-->
  ]]>

            </SCRIPT>

            <head>
                <title>.:: VISA ::.</title>
                <meta https-equiv="Content-Type" content="text/html; charset=iso-8859-1"/>
            </head>

            <link href="{$media_url}login/style.css" rel="stylesheet" type="text/css"/>

            <body onKeyPress="checkEnter();" bgcolor="#FFFFFF" text="#FFFFFF" link="#FFFFFF" vlink="#FFFFFF" alink="#FFFFFF">
                <table width="100%" border="0" cellspacing="0" cellpadding="0" align="center">
                    <table width="748" border="0" cellspacing="0" cellpadding="0" align="center" height="65">
                        <tr>
                            <td><img src="{$media_url}login/barrasup70.gif" width="743" height="70" align="right" usemap="#Map" border="0"/></td>
                        </tr>
                    </table>
                    <table style="border-bottom:#124c89 2px solid" width="745" border="0" cellspacing="0" cellpadding="0" align="center" height="400" background="{$media_url}login/fondo.gif">
                        <tr>
                            <td width="12" height="12">
                                <td width="463" height="12">
                                    <td width="6" height="12">
                                        <td width="250" height="12" valign="middle" align="right">

                                            <xsl:variable name="visahomenew-link">
                                                <xsl:value-of  select="seccionVisaHomeNew/link"/>
                                            </xsl:variable>

                                            <a href="javascript:openNewWindow('{$visahomenew-link}', 'thewin25', 'height=500,width=460,toolbar=no,scrollbars=no')" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('Image29','','{$media_url}login/seguridad2.gif',1)">
                                                <img src="{$media_url}login/seguridad1.gif" name="Image29" width="124" height="16" border="0"/>
                                            </a>

                                            <tr>
                                                <td width="12" rowspan="3">
                                                    <td height="2">
                                                        <td rowspan="3">
                                                            <td rowspan="5" width="267" valign="top">
                                                                <table width="256" border="0" cellspacing="0" cellpadding="0" height="400">
                                                                    <tr>
                                                                        <td width="19" height="19"><img src="{$media_url}login/puu_03.gif" width="19" height="19"/></td>
                                                                        <td width="221" background="{$media_url}login/boder_05.gif"></td>
                                                                        <td width="15"><img src="{$media_url}login/puu_07.gif" width="19" height="19"/></td>
                                                                    </tr>
                                                                    <tr>
                                                                        <td height="191" background="{$media_url}login/boder_14.gif"></td>
                                                                        <td width="221" bgcolor="#FBFBFB">
                                                                            <table width="217" border="0" cellspacing="0" cellpadding="0" height="275">
                                                                                <tr>
                                                                                    <td height="52" valign="middle" width="98">
                                                                                        <img src="{$media_url}login/visahome.gif" width="98" height="16"/>
                                                                                    </td>
                                                                                </tr>
                                                                                <tr>
                                                                                    <td valign="top" height="70" width="219"><font face="Verdana, Arial, Helvetica, sans-serif" size="1" color="#666666">
                                                                                        <xsl:value-of  select="seccionVisaHome/texto"/>
                                                                                    </font></td>
                                                                                </tr>
                                                                                <tr>
                                                                                    <td height="84" bgcolor="#F3F3F3" width="219" valign="top">

                                                                                        <form name="frm" onsubmit="return Validate(this)" action="Login.po" method="post" target="_top">
                                                                                            <!--input type="hidden" name="producto" value="vBusinessCommercial"/-->

                                                                                            <input type="hidden" name="event" value="login"/>
                                                                                            <input type="hidden" name="parameters" value="{$parameters}"/>
                                                                                            <input type="hidden" name="instancia" value="2"/>
                                                                                            <input type="hidden" name="marcaPrivada" value="999"/>
                                                                                            <input type="hidden" name="grupo" value="TH"/>
                                                                                            <input type="hidden" name="subGrupo" value="0"/>

                                                                                            <font size="1" face="Verdana, Arial, Helvetica, sans-serif" color="#666666">Tipo documento:</font><br/>
                                                                                            <select name="tipoDoc" height="10">
                                                                                                <option value="DNI" selected="true">Documento Nacional de Identidad</option>
                                                                                                <option value="PAS">Pasaporte</option>
                                                                                                <option value="DU">Documento Unico</option>
                                                                                                <option value="LC">Libreta Cívica</option>
                                                                                                <option value="LE">Libreta de Enrolamiento</option>
                                                                                                <option value="CI">Cédula de Identidad</option>
                                                                                            </select>
                                                                                            <table width="218" border="0" cellspacing="0" cellpadding="0" height="21">
                                                                                                <tr>
                                                                                                    <td width="109" bgcolor="#F3F3F3"><font size="1" face="Verdana, Arial, Helvetica, sans-serif" color="#666666">
                                                                                                        <div align="right">Número:</div></font>
                                                                                                    </td>
                                                                                                    <td width="109" bgcolor="#F3F3F3">
                                                                                                        <input type="text" name="usr" size="12"/>
                                                                                                    </td>
                                                                                                    <td width="30" bgcolor="#F3F3F3"></td>
                                                                                                </tr>
                                                                                                <tr>
                                                                                                    <td width="109" bgcolor="#F3F3F3">
                                                                                                        <font size="1" face="Verdana, Arial, Helvetica, sans-serif" color="#666666">
                                                                                                            <div align="right">Clave:</div>
                                                                                                        </font>
                                                                                                    </td>
                                                                                                    <td width="109" bgcolor="#F3F3F3">
                                                                                                        <input type="password" name="pwd" size="12"/>
                                                                                                    </td>
                                                                                                    <td width="30" bgcolor="#F3F3F3"></td>
                                                                                                </tr>
                                                                                                <tr>
                                                                                                    <td width="109" bgcolor="#F3F3F3">
                                                                                                    </td>
                                                                                                    <td width="109" bgcolor="#F3F3F3">
                                                                                                        <div align="right">
                                                                                                            <a href="javascript:window.document.frm.submit()">
                                                                                                                <img src="{$media_url}login/ingresar.gif" width="57" height="15"/>
                                                                                                            </a>
                                                                                                        </div>
                                                                                                    </td>
                                                                                                    <td width="30" bgcolor="#F3F3F3">
                                                                                                    </td>
                                                                                                </tr>
                                                                                            </table>
                                                                                        </form>

                                                                                    </td>
                                                                                </tr>
                                                                                <tr>
                                                                                    <td height="20" valign="top" width="219"><a href="https://inetserv.visa.com.ar/pid/app/GetBienvenida.event" target="_blank"><font face="Verdana, Arial, Helvetica, sans-serif" size="1" color="#789ab7">¿Olvidó
                                                                                        su clave de acceso?<img src="{$media_url}flecha.gif" width="10" height="8"/></font></a></td>
                                                                                </tr>
                                                                                <tr>
                                                                                    <td height="45" valign="top" width="219">
                                                                                        <p><font face="Verdana, Arial, Helvetica, sans-serif" size="1" color="#666666">
                                                                                            Si todavía no tiene clave de acceso,<br/>
                                                                                            comuníquese a los siguientes teléfonos:<br/>
                                                                                            <font color="#789ab7"><img src="{$media_url}login/icono.gif" width="7" height="7"/>Socios
                                                                                                Visa Gold</font> (011) 4378-4444<br/>
                                                                                            <font color="#789ab7"><img src="{$media_url}login/icono.gif" width="7" height="7"/>Socios
                                                                                                Visa Classic</font> (011) 4379-3400<br/>
                                                                                            <font color="#789ab7"><img src="{$media_url}login/icono.gif" width="7" height="7"/>Socios
                                                                                                Visa Platinium</font> 0800-333-7528<br/>
                                                                                            <font color="#789ab7"><img src="{$media_url}login/icono.gif" width="7" height="7"/>Commercial
                                                                                                Card</font> (011) 4379-3330</font></p>
                                                                                    </td>
                                                                                </tr>
                                                                            </table>
                                                                        </td>
                                                                        <td background="{$media_url}login/boder_12.gif"></td>
                                                                    </tr>
                                                                    <tr>
                                                                        <td width="19" height="2"><img src="{$media_url}login/puu_16.gif" width="19" height="19"/></td>
                                                                        <td width="221" background="{$media_url}login/boder_18.gif" height="2"></td>
                                                                        <td width="15" height="2"><img src="{$media_url}login/puu_20.gif" width="19" height="19"/></td>
                                                                    </tr>
                                                                </table>
                                                                <tr>
                                                                    <td valign="top" height="113">
                                                                        <table width="454" border="0" cellspacing="0" cellpadding="0" height="106">
                                                                            <tr>
                                                                                <td width="17" height="19"><img src="{$media_url}login/puu_03.gif" width="19" height="19"/></td>
                                                                                <td width="417" background="{$media_url}login/boder_05.gif"></td>
                                                                                <td width="20"><img src="{$media_url}login/puu_07.gif" width="19" height="19"/></td>
                                                                            </tr>
                                                                            <tr>
                                                                                <td width="17" background="{$media_url}login/boder_14.gif"></td>
                                                                                <td width="417" bgcolor="#FBFBFB">
                                                                                    <table width="350" border="0" cellspacing="0" cellpadding="0">
                                                                                        <tr>
                                                                                            <td width="85"><img src="{$media_url}login/beneficios.gif" width="66" height="39"/></td>
                                                                                            <xsl:variable name="promo-link1">
                                                                                                <xsl:value-of  select="seccionBeneficios1/link"/>
                                                                                            </xsl:variable>
                                                                                            <xsl:variable name="promo-link2">
                                                                                                <xsl:value-of  select="seccionBeneficios2/link"/>
                                                                                            </xsl:variable>
                                                                                            <td width="176"><a href="{$promo-link1}"><img src="{$media_url}bann1.gif" width="176" height="75"/></a></td>
                                                                                            <td width="176"><a href="{$promo-link2}"><img src="{$media_url}bann2.gif" width="176" height="75"/></a></td>
                                                                                            <!--td width="157"-->
                                                                                                <!--a href="http://www.visa.com.ar/Socios/Servicios/VentaPasajesBuses.htm"><img src="{$media_url}login/bann1.gif"/></a-->
                                                                                                <!--object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,29,0" width="169" height="69">
                                                                                                <param name="movie" value="{$media_url}login/banner.swf"/>
                                                                                                <param name="quality" value="high"/>
                                                                                                <embed src="{$media_url}login/banner.swf" quality="high" pluginspage="http://www.macromedia.com/go/getflashplayer" type="application/x-shockwave-flash" width="169" height="69"></embed></object-->
                                                                                            <!--/td-->
                                                                                            <!--td width="176"><a href="{$promo-link2}" target="_blank" class="unnamed1"><img src="{$media_url}login/bann2.gif" width="176" height="75"/></a></td-->
                                                                                        </tr>
                                                                                    </table>
                                                                                </td>
                                                                                <td width="20" background="{$media_url}login/boder_12.gif"></td>
                                                                            </tr>
                                                                            <tr>
                                                                                <td width="17" height="18"><img src="{$media_url}login/puu_16.gif" width="19" height="19"/></td>
                                                                                <td width="417" background="{$media_url}login/boder_18.gif" height="18"></td>
                                                                                <td width="20" height="18"><img src="{$media_url}login/puu_20.gif" width="19" height="19"/></td>
                                                                            </tr>
                                                                        </table>
                                                                        <font face="Verdana, Arial, Helvetica, sans-serif" size="1">
                                                                            <br/>
                                                                        </font>


                                                                        <table width="454" border="0" cellspacing="0" cellpadding="0" height="217">
                                                                            <tr>
                                                                                <td width="17" height="19"><img src="{$media_url}login/puu_03.gif" width="19" height="19"/></td>
                                                                                <td width="417" background="{$media_url}login/boder_05.gif"></td>
                                                                                <td width="20"><img src="{$media_url}login/puu_07.gif" width="19" height="19"/></td>
                                                                            </tr>
                                                                            <tr>
                                                                                <td width="17" background="{$media_url}login/boder_14.gif"></td>
                                                                                <td width="417" bgcolor="#FBFBFB" valign="bottom">
                                                                                    <table width="417" border="0" cellspacing="0" cellpadding="0" height="192">
                                                                                        <tr valign="top">
                                                                                            <td colspan="2" height="37"><img src="{$media_url}login/servicios.gif" width="78" height="16" align="top"/></td>
                                                                                        </tr>

                                                                                        <xsl:variable name="servicios-link1">
                                                                                            <xsl:value-of  select="seccionServicios1/link"/>
                                                                                        </xsl:variable>
                                                                                        <xsl:variable name="servicios-link2">
                                                                                            <xsl:value-of  select="seccionServicios2/link"/>
                                                                                        </xsl:variable>
                                                                                        <xsl:variable name="servicios-link3">
                                                                                            <xsl:value-of  select="seccionServicios3/link"/>
                                                                                        </xsl:variable>
                                                                                        <xsl:variable name="servicios-link4">
                                                                                            <xsl:value-of  select="seccionServicios4/link"/>
                                                                                        </xsl:variable>

                                                                                        <tr>
                                                                                            <td width="72" height="35"><a href="{$link4_1}" target="_blank" class="unnamed1"><img src="{$media_url}login/img4.gif" width="67" height="44"/></a></td>
                                                                                            <td height="35" width="345" valign="top"><a href="{$link4_1}" target="_blank" class="unnamed1"><font face="Verdana, Arial, Helvetica, sans-serif" size="1" color="#789AB7"><xsl:value-of  select="seccionServicios4/titulo"/></font></a>
                                                                                                <a href="{$link4_1}" target="_blank" class="unnamed1"><font face="Verdana, Arial, Helvetica, sans-serif" size="1"><br/><font color="#666666"><xsl:value-of  select="seccionServicios4/texto"/></font></font></a><br />   
                                                                                                <a href="{$link4_2}" target="_blank" class="unnamed1"><font face="Verdana, Arial, Helvetica, sans-serif" size="1" color="#666666">Ingrese <font face="Verdana, Arial, Helvetica, sans-serif" size="1"><font color="#789AB7">aqu&#237;</font></font>.</font></a>
                                                                                            </td>
                                                                                        </tr>

                                                                                        <tr>
                                                                                            <td width="72" height="35"><a href="{$link1}" target="_blank" class="unnamed1"><img src="{$media_url}login/img1.gif" width="67" height="44"/></a></td>
                                                                                            <td height="35" width="345" valign="top"><a href="{$link1}" target="_blank" class="unnamed1"><font face="Verdana, Arial, Helvetica, sans-serif" size="1" color="#789AB7"><xsl:value-of  select="seccionServicios1/titulo"/></font></a>
                                                                                                <a href="{$link1}" target="_blank" class="unnamed1"><font face="Verdana, Arial, Helvetica, sans-serif" size="1"><br/><font color="#666666"><xsl:value-of  select="seccionServicios1/texto"/></font></font></a>
                                                                                            </td>
                                                                                        </tr>

                                                                                        <tr>
                                                                                            <td width="72"><a href="{$link2}" target="_blank" class="unnamed1"><img src="{$media_url}login/img2.gif" width="67" height="44"/></a></td>
                                                                                            <td width="345" valign="top">
                                                                                                <a href="{$link2}" target="_blank" class="unnamed1">
                                                                                                    <font face="Verdana, Arial, Helvetica, sans-serif" size="1" color="#789ab7"><xsl:value-of  select="seccionServicios2/titulo"/></font></a>
                                                                                                <a href="{$link2}" target="_blank" class="unnamed1">
                                                                                                    <font face="Verdana, Arial, Helvetica, sans-serif" size="1" color="#666666"><br/>
                                                                                                        <xsl:value-of  select="seccionServicios2/texto"/>
                                                                                                    </font>
                                                                                                </a>
                                                                                            </td>
                                                                                        </tr>
                                                                                        <tr>
                                                                                            <td width="72"><a href="{$link3}" target="_blank" class="unnamed1"><img src="{$media_url}login/img3.gif" width="67" height="44"/></a></td>
                                                                                            <td width="345" valign="top">
                                                                                                <a href="{$link3}" target="_blank" class="unnamed1">
                                                                                                    <font size="1" face="Verdana, Arial, Helvetica, sans-serif" color="#789ab7"><xsl:value-of  select="seccionServicios3/titulo"/></font></a>
                                                                                                <a href="{$link3}" target="_blank" class="unnamed1">
                                                                                                    <font size="1" face="Verdana, Arial, Helvetica, sans-serif" color="#666666"><br/>
                                                                                                        <xsl:value-of  select="seccionServicios3/texto"/>
                                                                                                    </font>
                                                                                                </a>
                                                                                            </td>
                                                                                        </tr>
                                                                                    </table>
                                                                                </td>
                                                                                <td width="20" background="{$media_url}login/boder_12.gif"></td>
                                                                            </tr>
                                                                            <tr>
                                                                                <td width="17" height="2"><img src="{$media_url}login/puu_16.gif" width="19" height="19"/></td>
                                                                                <td width="417" background="{$media_url}login/boder_18.gif" height="2"></td>
                                                                                <td width="20" height="2"><img src="{$media_url}login/puu_20.gif" width="19" height="19"/></td>
                                                                            </tr>
                                                                        </table>
                                                                    </td>
                                                                </tr>
                                                            </td>
                                                        </td>
                                                    </td>
                                                </td>
                                            </tr>
                                        </td>
                                    </td>
                                </td>
                            </td>
                        </tr>
                    </table>
                    <table style="border-bottom:#ffffff 1px solid; border-top:#ffffff 1px solid" borderwidth="748" border="0" cellspacing="0" cellpadding="0" align="center" height="14" width="745" bordercolor="#FFFFFF">
                        <tr>
                            <td bgcolor="124c89" bordercolor="#FFFFFF" align="right" width="731"><font color="#FFFFFF" size="1" face="Verdana, Arial, Helvetica, sans-serif"><a href="http://www.visa.com.ar" class="unnamed1">www.visa.com.ar</a></font></td>
                            <td bgcolor="124c89" bordercolor="#FFFFFF" align="right" width="14"></td>
                        </tr>
                    </table>
                    <table style="border-top:#124c89 2px solid" width="745" border="0" cellspacing="0" cellpadding="0" align="center" height="28">
                        <tr>
                            <td height="28"><img src="{$media_url}login/barrabott.gif" width="745" height="28"/></td>
                        </tr>
                    </table>
                    <table width="748" border="0" cellspacing="0" cellpadding="0" align="center" height="20">
                        <tr>
                            <td><font face="Verdana, Arial, Helvetica, sans-serif" size="1" color="#999999">@
                                Copyright 2001 - Visa Argentina S.A.</font></td>
                        </tr>
                    </table>
                </table>
                <br/>
                <map name="Map">
                    <area shape="rect" coords="30,2,144,64" href="http://www.visa.com.ar" target="_self"/>
                </map>
            </body>
        </html>
    </xsl:template>
</xsl:stylesheet>
