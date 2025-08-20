project('Better Greeter', 'cpp',
        default_options: [ 'cpp_std=c++23', 'warning_level=3' ],
        version: '0.0.4',
        license: 'GPL-3.0-or-later',
        license_files: [ 'COPYING' ])

gnome = import('gnome')

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
    'src/greeter/greeter.cc',
    'src/greeter/window.cc',

    'src/widgets/profile.cc',

    'src/main.cc',
    'src/utils.cc',
)

resources = gnome.compile_resources(
    'resources',
    'resources.xml',
    source_dir: '.',
    c_name: 'app_resources',
    dependencies: files('greeter.xml'),
)

executable('better-greeter', source_files,
           include_directories: include_directories('include'),
           install: true)

install_data('style.css',
             install_dir: '/etc/greetd/better/')