<!--
 - Copyright (C) 2005, 2007  Internet Systems Consortium, Inc. ("ISC")
 -
 - Permission to use, copy, modify, and/or distribute this software for any
 - purpose with or without fee is hereby granted, provided that the above
 - copyright notice and this permission notice appear in all copies.
 -
 - THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
 - REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 - AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
 - INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 - LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 - OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 - PERFORMANCE OF THIS SOFTWARE.
-->

<!-- $Id: isc-manpage.xsl.in,v 1.9 2007-06-18 23:47:34 tbox Exp $ -->

<!-- ISC customizations for Docbook-XSL manual page generator. -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <!-- Import the Docbook manpages stuff -->
  <xsl:import href="manpages/docbook.xsl"/>
  
  <!-- Include our copyright generator -->
  <xsl:include href="copyright.xsl"/>

  <!-- Set comment string for this output format -->
  <xsl:param name="isc.copyright.leader">.\" </xsl:param>

  <!-- We're not writing any kind of SGML, thanks -->
  <xsl:output method="text" encoding="us-ascii"/>

  <!-- ANSI C function prototypes, please -->
  <xsl:param name="funcsynopsis.style">ansi</xsl:param>

  <!-- Use ranges when constructing copyrights -->
  <xsl:param name="make.year.ranges" select="1"/>

  <!-- Stuff we want in our nroff preamble. -->
  <xsl:variable name="isc.nroff.preamble">
    <xsl:text>.\"&#10;</xsl:text>
    <xsl:text>.\" &#36;Id&#36;&#10;</xsl:text>
    <xsl:text>.\"&#10;</xsl:text>
    <xsl:text>.hy 0&#10;</xsl:text>
    <xsl:text>.ad l&#10;</xsl:text>
  </xsl:variable>

  <!-- 
    - Override Docbook template to insert our copyright,
    - disable chunking, and suppress output of .so files.
   -->
  <xsl:template name="write.text.chunk">
    <xsl:param name="content"/>
    <xsl:if test="substring($content, 1, 4) != '.so ' or
		  substring-after($content, '&#10;') != ''">
      <xsl:call-template name="isc.no.blanks">
        <xsl:with-param name="text" select="
		concat($isc.copyright,
		       $isc.nroff.preamble,
		       $content)"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <!--
    - Suppress blank lines in nroff source we output.
   -->
  <xsl:template name="isc.no.blanks">
    <xsl:param name="text"/>
    <xsl:choose>
      <xsl:when test="contains($text, '&#10;')">
        <xsl:call-template name="isc.no.blanks">
	  <xsl:with-param name="text"
	                  select="substring-before($text, '&#10;')"/>
        </xsl:call-template>
	<xsl:call-template name="isc.no.blanks">
	  <xsl:with-param name="text"
			  select="substring-after($text, '&#10;')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="translate($text, '&#9;&#32;', '')">
        <xsl:value-of select="$text"/>
	<xsl:text>&#10;</xsl:text>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- 
    - Override Docbook template to change formatting.
    - We just want the element name in boldface, no subsection header.
   -->
  <xsl:template match="caution|important|note|tip|warning">
    <xsl:text>&#10;.RS&#10;.B "</xsl:text>
    <!-- capitalize word -->
    <xsl:value-of
      select="translate (substring (name(.), 1, 1), 'cintw', 'CINTW')" />
    <xsl:value-of select="substring (name(), 2)" />
    <xsl:if test="title">
      <xsl:text>: </xsl:text>
      <xsl:value-of select="title[1]"/>
    </xsl:if>
    <xsl:text>:"&#10;</xsl:text>
    <xsl:apply-templates/>
    <xsl:text>&#10;.RE&#10;</xsl:text>
  </xsl:template>

  <!--
    - Override template to change formatting.
    - We don't want hyphenation or justification.
   -->
  <xsl:template match="cmdsynopsis">
    <xsl:text>.HP </xsl:text>
    <xsl:value-of select="string-length (normalize-space (command)) + 1"/>
    <xsl:text>&#10;</xsl:text>
    <xsl:apply-templates/>
    <xsl:text>&#10;</xsl:text>
  </xsl:template>

  <!--
    - Override template to change formatting.
    - We don't want hyphenation or justification.
   -->
  <xsl:template match="funcsynopsis">
    <xsl:apply-templates/>
  </xsl:template>

  <!--
    - Override template to change formatting.
    - Line breaks in funcsynopsisinfo are significant.
   -->
  <xsl:template match="funcsynopsisinfo">
    <xsl:text>&#10;.nf&#10;</xsl:text>
    <xsl:apply-templates/>
    <xsl:text>&#10;.fi&#10;</xsl:text>
  </xsl:template>

</xsl:stylesheet>

<!-- 
  - Local variables:
  - mode: sgml
  - End:
 -->
