#!/bin/bash
#
#3> <> prov:specializationOf <https://github.com/tetherless-world/opendap/blob/master/opendap/bin/install-opendap.sh>;
#3>    prov:wasDerivedFrom   <https://github.com/tetherless-world/opendap/issues/7>;
#3>    rdfs:seeAlso          <https://github.com/tetherless-world/opendap/wiki/OPeNDAP-Installer#shell-based-installer>;
#3> .
#
# This script will install OPeNDAP from the current working directory.
#
# When run from:
#    /opt/opendap
#
# running:
#    install-opendap.sh ~/path/to/conf/hyrax-1.8_1.conf
#
# will create structure:
#    /opt/opendap/hyrax-1.8_1/hyrax-1.8_1.conf -(soft link)-> /full/path/to/conf/hyrax-1.8_1.conf
#    /opt/opendap/hyrax-1.8_1/bes-3.11.0/
#    /opt/opendap/hyrax-1.8_1/hdf5_handler-2.1.1
#    /opt/opendap/hyrax-1.8_1/libdap-3.11.7
#    ...
#    /opt/opendap/hyrax-1.8_1/hyrax-1.8_1.log

if [[ $# -lt 1 || "$1" == '--help' ]]; then
    echo "Usage:"
    echo "   `basename $0` <path/to/module-list.conf>"
    echo
    echo " <path/to/module-list.conf> - a file listing the <module>/<version> to install."
    echo "    e.g. https://raw.github.com/tetherless-world/opendap/master/opendap/conf/hyrax-1.8_1.conf"
    echo
    echo "see https://github.com/tetherless-world/opendap/wiki/OPeNDAP-Installer#shell-based-installer"
    exit
fi

configuration="$1"
# e.g. https://github.com/tetherless-world/opendap/blob/master/opendap/conf/hyrax-1.8_1.conf

tags="https://scm.opendap.org/svn/tags"
basedir=`pwd`

if [[ -e "$configuration" || "$configuration" =~ http* ]]; then
    # configuration = 
    # e.g. /path/to/hyrax-1.8_1.conf
    #      or as a URL:
    #      https://github.com/tetherless-world/opendap/blob/master/opendap/conf/hyrax-1.8_1.conf

    config_local="`basename $configuration`" # e.g. hyrax-1.8_1.conf
    config_id="${config_local%.*}"           # e.g. hyrax-1.8_1.conf -> hyrax-1.8_1
    mkdir -p "$config_id"                    # e.g.                     hyrax-1.8_1/

    if [[ -e "$configuration" ]]; then
        configuration_absolute=`readlink -e $configuration`
        ln -s "$configuration_absolute" "$config_id/$config_local"
    elif [[ "$configuration" =~ http* ]]; then
        curl -# -sL "$configuration" >  "$config_id/$config_local"
        configuration_absolute=`readlink -e $configuration`
        # Rewrite variable to be the local file, not the remote URL.
        configuration="$config_id/$config_local"
    fi

    log="${basedir}/${config_id}/${config_id}.log"
    echo "`basename $0` `date`" >> ${log}

   #
   # OPeNDAP Modules are listed at:
   # https://docs.google.com/spreadsheet/ccc?key=0An84UEjofnaydFRrUF9YWk03Y3NHNjJqUEg0NUhUZXc#gid=0
   #
   for module in `cat $configuration | grep -v '^#'`; do
      #
      # e.g. module = 'libdap/3.11.7'
      #
      component=`echo $module | cut -d'/' -f1`
      version=`echo $module | cut -d'/' -f2`
      url="${tags}/${component}/${version}"
	  otherconf=
      echo svn co $url "$config_id/$component-$version"
           svn co $url "$config_id/$component-$version"
      if [[ "$component" == 'libdap' ]]; then
            echo hi
            # TODO: check which and offer to apt-get if not exists.
            # More coplete list of dependencies and how to resolve at:
            # https://hackpad.com/OPeNDAPPROV-2014-Jan-13-pw36WNo46OU
            # automake
            # autoconf
            # libtool
            # m4
            # g++
            # flex
            # bison
            # doxygen
            # libcurl
         if [[ ! `which curl-config` ]]; then
            echo "TODO: install libcurl"
         else
            echo "$component needs libcurl; it is installed at `which curl-config`"
         fi
		 if [[ ! -e /usr/include/uuid/uuid.h ]]; then
			echo "TODO: install uuid-dev"
		 else
		    echo "$component needs uuid.h; it is installed at /usr/include/uuid"
		 fi
      elif [[ "$component" == 'bes' ]]; then
         echo we need ...
		 otherconf="--enable-developer"
      elif [[ "$component" == 'dap-server' ]]; then
         echo we need ...
      fi
	  (cd $config_id/$component-$version
		  autoreconf --force --install 2>&1 | tee -a ${log}
		  if [ $? != 0 ]
		  then
			echo "autoreconf failed dude"
			exit 1
		  fi
		  ./configure --prefix=${basedir}/${config_id} $otherconf 2>&1 | tee -a ${log}
		  if [ $? != 0 ]
		  then
			echo "configure failed dude"
			exit 1
		  fi
		  make 2>&1 | tee -a ${log}
		  if [ $? != 0 ]
		  then
			echo "make failed dude"
			exit 1
		  fi
		  make check 2>&1 | tee -a ${log}
		  if [ $? != 0 ]
		  then
			echo "make check failed dude"
			exit 1
		  fi
		  make install 2>&1 | tee -a ${log}
		  if [ $? != 0 ]
		  then
			echo "make install failed dude"
			exit 1
		  fi
	  )
   done
fi

