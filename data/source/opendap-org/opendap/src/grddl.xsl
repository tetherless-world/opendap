<xsl:transform version="2.0" 
   xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
   xmlns:xs="http://www.w3.org/2001/XMLSchema"
   exclude-result-prefixes="">
<xsl:output method="text"/>

<xsl:param name="cr-base-uri"   select="'http://opendap.tw.rpi.edu'"/>
<xsl:param name="cr-source-id"  select="'opendap-org'"/>
<xsl:param name="cr-dataset-id" select="'opendap'"/>
<xsl:param name="cr-version-id" select="'svn'"/>

<xsl:variable name="s"   select="concat($cr-base-uri,'/source/',$cr-source-id,'/')"/>
<xsl:variable name="sd"  select="concat($s,'dataset/',$cr-dataset-id,'/')"/>
<xsl:variable name="sdv" select="concat($sd,'version/',$cr-version-id,'/')"/>

<xsl:variable name="svn" select="'https://scm.opendap.org/svn'"/>

<xsl:variable name="prefixes"><xsl:text><![CDATA[@prefix prov:   <http://www.w3.org/ns/prov#>.
@prefix rdfs:   <http://www.w3.org/2000/01/rdf-schema#>.
@prefix xsd:    <http://www.w3.org/2001/XMLSchema#>.
@prefix schema: <http://schema.org/>.
]]>
</xsl:text>
</xsl:variable>

<xsl:template match="/">
   <xsl:value-of select="$prefixes"/>

   <xsl:for-each-group select="log/logentry/paths/path" group-by="@kind">
      <xsl:message select="concat('kind: ',current-grouping-key())"/>
   </xsl:for-each-group>

   <xsl:for-each-group select="log/logentry" group-by="author">
      <xsl:message select="concat('author: ',current-grouping-key())"/>
   </xsl:for-each-group>

   <xsl:message select="concat('num no authors: ',count(log/logentry[not(author)]))"/>

   <xsl:for-each-group select="log/logentry/paths/path" group-by="@action">
      <xsl:message select="concat('action: ',current-grouping-key())"/>
   </xsl:for-each-group>

   <xsl:apply-templates select="//logentry">
      <xsl:sort select="number(@revision)" order="descending"/>
   </xsl:apply-templates>
</xsl:template>

<xsl:template match="logentry">
   <xsl:value-of select="concat('&lt;',$sd,'commit/',@revision,'&gt;',$NL,
                                '   a prov:Activity;',$NL)"/>

   <xsl:if test="string-length(author/text())">
      <xsl:value-of select="concat('   prov:wasAttributedTo &lt;',$s,'id/developer/',author,'&gt;;',$NL)"/>
   </xsl:if>

   <xsl:if test="string-length(date/text())">
      <xsl:value-of select="concat('   prov:endedAtTime ',$DQ,date,$DQ,'^^xsd:dateTime;',$NL)"/>
   </xsl:if>

   <!-- paths -->
   <xsl:for-each select="paths/path[not(contains(.,'&gt;'))]">
      <xsl:variable name="path"     select="replace(.,' ','%20')"/>
      <xsl:variable name="revision" select="concat('&lt;',$sd,'revision/',../../@revision,$path,'&gt;')"/>
      <xsl:value-of select="concat('   prov:generated ',$revision,';',$NL)"/>
   </xsl:for-each>

   <xsl:if test="string-length(msg/text())">
      <xsl:value-of select="concat('   rdfs:comment ',$DQ,$DQ,$DQ,replace(
                                                                  replace(
                                                                  replace(msg,'\\', '\\\\'            ),
                                                                              $DQ, concat('\\',$DQ) ),
                                                                              '/','\\/'           ),
                                                      $DQ,$DQ,$DQ,';',$NL)"/>
   </xsl:if>

   <xsl:value-of select="concat('.',$NL)"/>

   <!-- paths -->
   <xsl:for-each select="paths/path[not(contains(.,'&gt;'))]">
      <xsl:variable name="path"     select="replace(.,' ','%20')"/>
      <xsl:variable name="revision" select="concat('&lt;',$sd,'revision/',../../@revision,$path,'&gt;')"/>
      <xsl:value-of select="concat($NL,$revision,$NL,
                                   '   a prov:Entity;',$NL,
                                   '   schema:version ',$DQ,../../@revision,$DQ,';',$NL,
                                   '   prov:specializationOf &lt;',$svn,$path,'&gt;;',$NL,
                                   '.')"/>
   </xsl:for-each>

   <xsl:value-of select="concat($NL,$NL,'# ^ ^ - - ',@revision,' - - ^ ^',$NL,$NL)"/>
</xsl:template>

<xsl:template match="@*|node()">
  <xsl:copy>
      <xsl:copy-of select="@*"/>   
      <xsl:apply-templates/>
  </xsl:copy>
</xsl:template>

<!--xsl:template match="text()">
   <xsl:value-of select="normalize-space(.)"/>
</xsl:template-->

<xsl:variable name="NL" select="'&#xa;'"/>
<xsl:variable name="DQ" select="'&#x22;'"/>

</xsl:transform>
