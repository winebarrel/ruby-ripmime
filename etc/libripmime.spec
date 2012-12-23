%define exec_name ripmime
%define mejor_version 1.4.0

Name:		libripmime
Version:	1.4.0.10
Release:	1%{?dist}
Summary:	Extract attachments out of a MIME encoded email packages

Group:		Development/Libraries
License:	BSD
URL:		http://www.pldaniels.com/ripmime/
Source0:	http://www.pldaniels.com/%{name}/%{exec_name}-%{version}.tar.gz
BuildRoot:	%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

%description
ripMIME extract attachments out of a MIME encoded email packages.

%prep
%setup -q -n %{exec_name}-%{version}


%build
make %{?_smp_mflags} CFLAGS="-Wall -g -I. -O2 -Werror -fPIC" solib


%install
rm -rf %{buildroot}
install -Dp -m 0644 $RPM_BUILD_DIR/%{exec_name}-%{version}/%{name}.so.%{mejor_version} %{buildroot}%{_libdir}/%{name}.so.%{mejor_version}
install -Dp -m 0644 $RPM_BUILD_DIR/%{exec_name}-%{version}/%{exec_name}-api.h %{buildroot}%{_includedir}/%{exec_name}-api.h
cd %{buildroot}%{_libdir}; ln -sf %{name}.so.%{mejor_version} %{name}.so


%clean
rm -rf %{buildroot}


%files
%defattr(-,root,root,-)
%{_libdir}/%{name}.so.%{mejor_version}
%{_libdir}/%{name}.so
%{_includedir}/%{exec_name}-api.h
%doc CHANGELOG CONTRIBUTORS INSTALL LICENSE TODO README


%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig
