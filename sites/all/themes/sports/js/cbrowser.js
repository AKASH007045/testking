function cVersion ( version, separator, bSkipSpace )
{
if ( arguments.length < 1 )
    version = '0';
if ( arguments.length < 2 )
    separator = '.';
if ( arguments.length < 3 )
    bSkipSpace = false;
if ( version instanceof cVersion )
  {
    this.separator = (arguments.length < 2) ? version.separator : separator;
    this.v = new Array();
    this.v = version.v;
  }
else
  {
    var s = ( typeof(version) == 'number' ) ? version.toString() : version;
    this.separator = separator;
    this.v = new Array();
    var vindex = 0;
    var sindex = 0;
    var c;
    this.v[vindex] = '';
    if ( bSkipSpace) for ( ; sindex < s.length; ++sindex )
      {
        c = s.charAt(sindex);
        if ( c != ' ' )
            break;
      }
    for ( ; sindex < s.length; ++sindex )
      {
        c = s.charAt(sindex);
        if ( c == separator )
            this.v[++vindex] = '';
        else if ( (c >= '0') && (c <= '9') )
            this.v[vindex] += c.toString();
        else
            break;
      }
  }
return;
}

cVersion.prototype.toString = function ( separator )
{
if ( arguments.length < 1 )
    separator = this.separator;
var rv = '';
for ( var i = 0; i < this.v.length; ++i )
  {
    if ( i == 0 )
        rv += this.v[0];
    else
        rv += separator + this.v[i];
  }
return rv;
}

cVersion.prototype.comp = function ( version2 )
{
var operand;
if ( arguments.length < 1 )
    version2 = '0';
if ( version2 instanceof cVersion )
    operand = version2;
else
    operand = new cVersion( version2 );
var nLoops = Math.max( this.v.length, operand.v.length );
var rv = 0;
for ( var i = 0; i < nLoops; ++i )
  {
    var n1 = Number( (i < this.v.length) ? this.v[i] : 0 );
    var n2 = Number( (i < operand.v.length) ? operand.v[i] : 0 );
    if ( n1 == n2 )
        continue;
    else if ( n1 < n2 )
        { rv = -1; break; }
    else
        { rv = 1; break; }
  }
return rv;
}

function cBrowser ( ua, ver )
{
if ( arguments.length == 0 )
    ua = navigator.userAgent;
this.ua = ua.toLowerCase();
if ( arguments.length < 2 )
    ver = navigator.appVersion;
this.isIE8 = this.isIE9 = this.isIE10 = this.isIE11 = false;
if ( typeof(isTrulyIE) != 'undefined' )
  {
    this.isIE = true;
    this.isKhtml = this.isChrome = this.isSafari = this.isOpera = this.isGecko = this.isNetscape = this.isWebtv = false;
  }
else
  {
    this.isKhtml = (this.ua.indexOf("khtml") != -1);
    this.isChrome = (this.ua.indexOf("chrome/") != -1);
    this.isSafari = (this.ua.indexOf("safari") != -1);
    this.isOpera = (this.ua.indexOf("opera") != -1) || (this.ua.indexOf("opr/") != -1);
    this.isGecko = !this.isKhtml && !this.isChrome && !this.isSafari && !this.isOpera &&
        (this.ua.indexOf("gecko/") != -1);
    this.isNetscape = !this.isKhtml && !this.isChrome && !this.isSafari && !this.isOpera &&
      ( (this.ua.indexOf('mozilla')!=-1) &&
        ((this.ua.indexOf('spoofer')==-1) &&
        (this.ua.indexOf('trident')==-1) &&
        (this.ua.indexOf('compatible')==-1)) );
    this.isWebtv = (this.ua.indexOf("webtv")!=-1);
    this.isIE = !this.isWebtv && !this.isOpera &&
      ( (this.ua.indexOf('msie') != -1) || (this.ua.indexOf('trident') != -1) );
    this.version = ver;
    if ( this.isNetscape && (this.version>=5) )
        this.isGecko = true;
  }
if ( this.isIE )
  {
    if ( this.ua.indexOf('rv:') != -1 )
        this.version = this.ua.substring(3+this.ua.indexOf("rv:"));
    else
        this.version = parseFloat(this.ua.substring(4+this.ua.indexOf("msie")));
    this.isIE8 = (this.version == "8.0") || (this.ua.indexOf("trident/4") != -1);
    this.isIE9 = (this.version == "9.0") || (this.ua.indexOf("trident/5") != -1);
    this.isIE10 = (this.version == "10.0") || (this.ua.indexOf("trident/6") != -1);
    this.isIE11 = (this.version == "11.0") || (this.ua.indexOf("trident/7") != -1);
    this.version = new cVersion( this.version );
  }
else if ( this.isOpera )
  {
    if ( this.ua.indexOf('version/') != -1 )
        this.version = new cVersion( parseFloat(this.ua.substring(8+this.ua.indexOf("version/"))) );
    else if ( this.ua.indexOf('opera/') == 0 )
        this.version = new cVersion( parseFloat(this.ua.substring(1+this.ua.indexOf("/"))) );
    else if ( this.ua.indexOf("opera/") != -1 )
        this.version = new cVersion( parseFloat(this.ua.substring(2+this.ua.indexOf(")"))) );
    else if ( this.ua.indexOf("opr/") != -1 )
        this.version = new cVersion( this.ua.substring(4+this.ua.indexOf("opr/")) );
    else
        this.version = new cVersion( parseFloat(this.ua.substring(6+this.ua.indexOf("opera/"))) );
  }
else
  {
    this.version = new cVersion( this.version );
  }
}
