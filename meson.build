project('Better Greeter', 'cpp',
        default_options: [ 'cpp_std=c++23', 'warning_level=3' ],
        version: '0.1.2',
        license: 'GPL-3.0-or-later',
        license_files: [ 'COPYING' ])

add_project_dependencies([
    dependency('gtkmm-4.0'),
    dependency('jsoncpp')
], language: 'cpp')

add_project_arguments([
    '-DAPP_VERSION="' + meson.project_version() + '"',
    '-DAPP_NAME="'    + meson.project_name()    + '"',
    '-DAPP_ID="org.BetterDE.Better-Greeter"',
], language: 'cpp')

source_files = files(
    'src/greeter/app.cc',
    'src/greeter/interface.cc',
    'src/greeter/ipc.cc',
    'src/greeter/session.cc',
    'src/greeter/utils.cc',

    'src/main.cc',
)

executable('better-greeter', source_files,
           include_directories: include_directories('include'),
           install: true)

install_data('style.css', 'greeter.xml',
             install_dir: '/etc/better/greeter')