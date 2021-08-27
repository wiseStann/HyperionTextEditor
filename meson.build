project('Hyperion Text Editor', 'c')

executable (
        'Hyperion',

        # source files must be specified explicitly
        # read more here https://mesonbuild.com/FAQ.html#why-cant-i-specify-target-files-with-a-wildcard 
        sources: ['src/hyper_app.c', 'src/hyper_app_win.c', 'src/hyper_app_prefs.c', 'src/resources.c', 'main.c'],
        
        dependencies: dependency('gtk4', required : true),
    )