Name:	monikor
Version: 1.2.1
Release: 1%{?dist}
Summary: Quanta monitoring packages
Distribution: Quanta
Vendor: Quanta-Computing
Packager: Matthieu ROSINSKI <support@quanta.io>
Group: Applications/Internet
URL: http://www.quanta.io
License: toto
Source: monikor-1.2.1.tgz

Buildroot	: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
%define _unpackaged_files_terminate_build 0
%define _missing_doc_files_terminate_build 0
%global __requires_exclude_from %{arch_exclude_files_from_autoreq}|%{common_exclude_files_from_autoreq}|^.*libcurl\\.so\\.4.*$

%description
Quanta monitors systems and applications. This series of packages provides the
correct tools to collect and send metrics to Quanta servers.

%package -n quanta-agent-base
Summary: Quanta agent with no plugins
Group: Applications/Internet
BuildRequires : libcurl-devel
BuildRequires : libyaml-devel
Requires: libmonikor = %{version}

%package -n libmonikor
Summary: Metric library for quanta-agent
Group: Applications/Internet

%package -n libmonikor-devel
Summary: Development package for libmonikor
Group: Applications/Internet
Requires: libmonikor = %{version}

%package -n quanta-agent-apache
Summary: Quanta agent apache plugin
Group: Applications/Internet
Requires: quanta-agent-base = %{version}

%package -n quanta-agent-cpu
Summary: Quanta agent CPU plugin
Group: Applications/Internet
Requires: quanta-agent-base = %{version}

%package -n quanta-agent-disk
Summary: Quanta agent disk plugin
Group: Applications/Internet
Requires: quanta-agent-base = %{version}

%package -n quanta-agent-memcached
Summary: Quanta agent memcached plugin
Group: Applications/Internet
Requires: quanta-agent-base = %{version}

%package -n quanta-agent-memory
Summary: Quanta agent memory plugin
Group: Applications/Internet
Requires: quanta-agent-base = %{version}

%package -n quanta-agent-mysql
Summary: Quanta agent Mysql plugin
Group: Applications/Internet
BuildRequires : mysql-devel
Requires: mysql-libs
Requires: quanta-agent-base = %{version}

%package -n quanta-agent-network
Summary: Quanta agent network plugin
Group: Applications/Internet
Requires: quanta-agent-base = %{version}

%package -n quanta-agent-nfs
Summary: Quanta agent NFS plugin
Group: Applications/Internet
Requires: quanta-agent-base = %{version}

%package -n quanta-agent-nginx
Summary: Quanta agent Nginx plugin
Group: Applications/Internet
Requires: quanta-agent-base = %{version}

%package -n quanta-agent-postgresql
Summary: Quanta agent PostgreSQL plugin
Group: Applications/Internet
BuildRequires: postgresql-devel
Requires: postgresql-libs
Requires: quanta-agent-base = %{version}

%package -n quanta-agent-process
Summary: Quanta agent process plugin
Group: Applications/Internet
Requires: quanta-agent-base = %{version}

%package -n quanta-agent-redis
Summary: Quanta agent Redis plugin
Group: Applications/Internet
Requires: quanta-agent-base = %{version}

%package -n quanta-agent-system
Summary: Quanta agent system plugin
Group: Applications/Internet
Requires: quanta-agent-base = %{version}

%package -n quanta-agent-varnish
Summary: Quanta agent Varnish plugin
Group: Applications/Internet
BuildRequires: varnish-libs-devel
Requires: varnish-libs >= 4.0.0
Requires: quanta-agent-base = %{version}

%description -n quanta-agent-base
This package provides a daemon to monitor system health and reports to Quanta servers.

%description -n libmonikor
This package provides a metric library for quanta-agent

%description -n libmonikor-devel
This package provides a development headers for libmonikor

%description -n quanta-agent-apache
This package provides a Apache2 plugin for quanta-agent

%description -n quanta-agent-cpu
This package provides a CPU plugin for quanta-agent

%description -n quanta-agent-disk
This package provides a disk plugin for quanta-agent

%description -n quanta-agent-memcached
This package provides a memcached plugin for quanta-agent

%description -n quanta-agent-memory
This package provides a memory plugin for quanta-agent

%description -n quanta-agent-mysql
This package provides a Mysql plugin for quanta-agent

%description -n quanta-agent-network
This package provides a network plugin for quanta-agent

%description -n quanta-agent-nfs
This package provides a NFS plugin for quanta-agent

%description -n quanta-agent-nginx
This package provides a Nginx plugin for quanta-agent

%description -n quanta-agent-postgresql
This package provides a PostgreSQL plugin for quanta-agent

%description -n quanta-agent-process
This package provides a process plugin for quanta-agent

%description -n quanta-agent-redis
This package provides a Redis plugin for quanta-agent

%description -n quanta-agent-system
This package provides a system plugin for quanta-agent

%description -n quanta-agent-varnish
This package provides a Varnish plugin for quanta-agent


%prep
%setup

%build
%configure
sed -i 's|^hardcode_libdir_flag_spec=.*|hardcode_libdir_flag_spec=""|g' libtool
sed -i 's|^runpath_var=LD_RUN_PATH|runpath_var=DIE_RPATH_DIE|g' libtool
make

%install
%makeinstall
mkdir -p %{buildroot}/etc/quanta/modules.d
mkdir -p %{buildroot}/usr/lib/quanta/modules
mkdir -p %{buildroot}/etc/init.d

install -m 755 misc/monikor.centos.init %{buildroot}/etc/init.d/quanta-agent
mv %{buildroot}/usr/bin/monikor %{buildroot}/usr/bin/quanta-agent
mv %{buildroot}/etc/monikor/config.yml %{buildroot}/etc/quanta/agent.yml
mv %{buildroot}/etc/monikor/modules.d/*.yml %{buildroot}/etc/quanta/modules.d/.
mv %{buildroot}/usr/lib64/apache.so %{buildroot}/usr/lib/quanta/modules/apache.so
mv %{buildroot}/usr/lib64/cpu.so %{buildroot}/usr/lib/quanta/modules/cpu.so
mv %{buildroot}/usr/lib64/disk.so %{buildroot}/usr/lib/quanta/modules/disk.so
mv %{buildroot}/usr/lib64/memcached.so %{buildroot}/usr/lib/quanta/modules/memcached.so
mv %{buildroot}/usr/lib64/memory.so %{buildroot}/usr/lib/quanta/modules/memory.so
mv %{buildroot}/usr/lib64/mysqlstat.so %{buildroot}/usr/lib/quanta/modules/mysqlstat.so
mv %{buildroot}/usr/lib64/network.so %{buildroot}/usr/lib/quanta/modules/network.so
mv %{buildroot}/usr/lib64/nfs.so %{buildroot}/usr/lib/quanta/modules/nfs.so
mv %{buildroot}/usr/lib64/nginx.so %{buildroot}/usr/lib/quanta/modules/nginx.so
mv %{buildroot}/usr/lib64/postgresql.so %{buildroot}/usr/lib/quanta/modules/postgresql.so
mv %{buildroot}/usr/lib64/process.so %{buildroot}/usr/lib/quanta/modules/process.so
mv %{buildroot}/usr/lib64/redis.so %{buildroot}/usr/lib/quanta/modules/redis.so
mv %{buildroot}/usr/lib64/system.so %{buildroot}/usr/lib/quanta/modules/system.so
mv %{buildroot}/usr/lib64/varnish.so %{buildroot}/usr/lib/quanta/modules/varnish.so

# remove unused lib files
rm -f %{buildroot}/usr/lib64/*.la
rm -f %{buildroot}/usr/lib64/*.a

%files -n quanta-agent-base
/etc/init.d/quanta-agent
/usr/bin/quanta-agent
%config(noreplace) /etc/quanta/agent.yml
%dir /etc/quanta/modules.d

%files -n libmonikor
/usr/lib64/libmonikor.so*

%files -n libmonikor-devel
/usr/include/monikor/metric.h

%files -n quanta-agent-apache
/usr/lib/quanta/modules/apache.so
%config(noreplace) /etc/quanta/modules.d/apache.yml

%files -n quanta-agent-cpu
/usr/lib/quanta/modules/cpu.so
%config(noreplace) /etc/quanta/modules.d/cpu.yml

%files -n quanta-agent-disk
/usr/lib/quanta/modules/disk.so
%config(noreplace) /etc/quanta/modules.d/disk.yml

%files -n quanta-agent-memcached
/usr/lib/quanta/modules/memcached.so
%config(noreplace) /etc/quanta/modules.d/memcached.yml

%files -n quanta-agent-memory
/usr/lib/quanta/modules/memory.so
%config(noreplace) /etc/quanta/modules.d/memory.yml

%files -n quanta-agent-mysql
/usr/lib/quanta/modules/mysqlstat.so
%config(noreplace) /etc/quanta/modules.d/mysqlstat.yml

%files -n quanta-agent-network
/usr/lib/quanta/modules/network.so
%config(noreplace) /etc/quanta/modules.d/network.yml

%files -n quanta-agent-nfs
/usr/lib/quanta/modules/nfs.so
%config(noreplace) /etc/quanta/modules.d/nfs.yml

%files -n quanta-agent-nginx
/usr/lib/quanta/modules/nginx.so
%config(noreplace) /etc/quanta/modules.d/nginx.yml

%files -n quanta-agent-postgresql
/usr/lib/quanta/modules/postgresql.so
%config(noreplace) /etc/quanta/modules.d/postgresql.yml

%files -n quanta-agent-process
/usr/lib/quanta/modules/process.so
%config(noreplace) /etc/quanta/modules.d/process.yml

%files -n quanta-agent-redis
/usr/lib/quanta/modules/redis.so
%config(noreplace) /etc/quanta/modules.d/redis.yml

%files -n quanta-agent-system
/usr/lib/quanta/modules/system.so
%config(noreplace) /etc/quanta/modules.d/system.yml

%files -n quanta-agent-varnish
/usr/lib/quanta/modules/varnish.so
%config(noreplace) /etc/quanta/modules.d/varnish.yml

%post -n quanta-agent-base
HOSTID=`ip link | grep link/ether | head -1 | awk '{print $2}' | tr -d ':'`
if [ -f /etc/quanta/agent.yml ]; then
  sed -i -e s/__YOUR_HOSTID__/$HOSTID/ /etc/quanta/agent.yml
fi
getent group quanta-agent > /dev/null || groupadd -r quanta-agent
getent passwd quanta-agent > /dev/null || \
    useradd -r -g quanta-agent -d /var/run/quanta -s /sbin/nologin \
    -c "Quanta agent" quanta-agent
:
if [ ! -d /var/run/quanta ]; then
  mkdir /var/run/quanta
  chown quanta-agent:quanta-agent /var/run/quanta
fi

%postun -n quanta-agent-base
if getent passwd quanta-agent >/dev/null 2>&1; then
  userdel quanta-agent
fi
if getent group quanta-agent >/dev/null 2>&1; then
  groupdel quanta-agent
fi
rm -Rf /var/run/quanta
