<!doctype html>
<html class="no-js">
    <head>
        <meta charset="utf-8">
        <meta http-equiv="x-ua-compatible" content="ie=edge">
        <link rel="shortcut icon" type="image/x-icon" href="/favicon.ico" />
        <title>
        
    random_r.c - stdlib/random_r.c -  Glibc source code glibc-2.42.9000 - Bootlin Elixir Cross Referencer

        </title>
        <meta name="description"
            content="Elixir Cross Referencer - source code of Glibc glibc-2.42.9000: stdlib/random_r.c">
        <meta name="viewport" content="user-scalable=no, initial-scale=1, maximum-scale=1, minimum-scale=1" />
        <link rel="stylesheet" href="/static/style.css?v=16">

        <link rel="preload" href="/static/img/2penguins.svg" as="image" />
        <link rel="preload" href="/static/img/arrow-dropdown-16.svg" as="image" />

        <link rel="preload" crossorigin href="/static/fonts/lxr.ttf" as="font" />
        <link rel="preload" crossorigin href="/static/fonts/ubuntu/ubuntu-v14-cyrillic_greek_latin_latin-ext_greek-ext_cyrillic-ext-regular.woff2" as="font" />
        <link rel="preload" crossorigin href="/static/fonts/ubuntu/ubuntu-v14-cyrillic_greek_latin_latin-ext_greek-ext_cyrillic-ext-700.woff2" as="font" />
        <link rel="preload" crossorigin href="/static/fonts/ubuntu/ubuntu-v14-cyrillic_greek_latin_latin-ext_greek-ext_cyrillic-ext-italic.woff2" as="font" />

        <link rel="preload" crossorigin href="/static/messages.json" as="fetch" />

        <script>
            document.documentElement.classList.remove('no-js');
            document.documentElement.classList.add('js');

            //NOTE: placing this here, instead of script.js makes the sidebar less likely to flash on navigation
            const showSidebarStr = window.localStorage.getItem('show-sidebar');
            const showSidebar = showSidebarStr == "true" || showSidebarStr == null;
            if (showSidebar) {
              // special class that opens the sidebar only on widescreen devices
              document.documentElement.classList.add('show-menu');
            }
        </script>
    </head>
    <body>
        <!--[if lte IE 9]>
                <p class="browserupgrade">You are using an <strong>outdated</strong> browser. Please <a href="http://browsehappy.com/">upgrade your browser</a> to improve your experience and security.</p>
        <![endif]-->
        <div class="wrapper">
            <header class="header">
    <nav>
        <ul class="nav-links">
            <li><a href="https://bootlin.com/">Home</a></li>
            <li><a href="https://bootlin.com/engineering/">Engineering</a></li>
            <li><a href="https://bootlin.com/training/">Training</a></li>
            <li><a href="https://bootlin.com/docs/">Docs</a></li>
            <li><a href="https://bootlin.com/community/">Community</a></li>
            <li><a href="https://bootlin.com/company/">Company</a></li>
        </ul>
        <ul class="social-icons">
            <li><a target="_blank" class="icon-twitter" href="http://twitter.com/bootlincom">twitter</a></li>
            <li><a target="_blank" class="icon-mastodon" href="https://fosstodon.org/@bootlin">mastodon</a></li>
            <li><a target="_blank" class="icon-linkedin" href="https://www.linkedin.com/company/bootlin/">linkedin</a></li>
            <li><a target="_blank" class="icon-github" href="https://github.com/bootlin">github</a></li>
        </ul>
    </nav>
    <div class="header-main">
        <div class="banners">
            <a class="message-link" target="_blank" href="https://bootlin.com/">
                <div class="message-banner">
                    <div class="container">
                        <p class="title"></p>
                        <div class="subtitle"></div>
                        <div class="action">
                            <div class="action-inner">
                            </div>
                        </div>
                    </div>
                </div>
            </a>
        </div>
        <div class="header-logo">
            <h1>
                <img src="/static/img/bootlin-logo-white.svg" alt="Bootlin logo"/>
            </h1>
            <h2>Elixir Cross Referencer</h2>
        </div>
        <div></div>
    </div>
</header>
            <header class="topbar">
    <div class="breadcrumb">
        <a href="#menu" class="open-menu icon-menu screenreader" title="Open Menu">Open Menu</a>
        <a class="project" href="/glibc/glibc-2.42.9000/source">/</a>
        
            <a href="/glibc/glibc-2.42.9000/source/stdlib">stdlib</a>
            /
        
            <a href="/glibc/glibc-2.42.9000/source/stdlib/random_r.c">random_r.c</a>
            
        
    </div>
    <div class="search">
        <form method="GET" action="/glibc/glibc-2.42.9000/ident" id="search-form">
            <select name="f" title="Restricts search to specific file families">
            
                <option value="A" selected>
                All symbols
                </option>
            
                <option value="C" >
                C/CPP/ASM
                </option>
            
                <option value="K" >
                Kconfig
                </option>
            
                <option value="D" >
                Devicetree
                </option>
            
                <option value="B" >
                DT compatible
                </option>
            
            </select>

            <div class="search-controls">
                <input placeholder="Search Identifier" type="text" id="search-input" name="i"
                    value="" autofocus required/>
                <button type="button" id="clear-search" class="visible-if-js icon-cross"><span class="screenreader">Clear</span></button>
                <button type="submit" class="icon-search"><span class="screenreader">Go get it</span></button>
            </div>
        </form>
    </div>
</header>
            <main>
                <aside class="sidebar" id="menu">
    <div class="filter">
        <select class="select-projects" onchange="window.location.href=this.value">
            <option  value="/amazon-freertos/latest/source">amazon-freertos</option>
            <option  value="/arm-trusted-firmware/latest/source">arm-trusted-firmware</option>
            <option  value="/barebox/latest/source">barebox</option>
            <option  value="/bluez/latest/source">bluez</option>
            <option  value="/busybox/latest/source">busybox</option>
            <option  value="/coreboot/latest/source">coreboot</option>
            <option  value="/dpdk/latest/source">dpdk</option>
            <option  value="/freebsd/latest/source">freebsd</option>
            <option selected value="/glibc/latest/source">glibc</option>
            <option  value="/grub/latest/source">grub</option>
            <option  value="/iproute2/latest/source">iproute2</option>
            <option  value="/linux/latest/source">linux</option>
            <option  value="/llvm/latest/source">llvm</option>
            <option  value="/mesa/latest/source">mesa</option>
            <option  value="/musl/latest/source">musl</option>
            <option  value="/ofono/latest/source">ofono</option>
            <option  value="/op-tee/latest/source">op-tee</option>
            <option  value="/opensbi/latest/source">opensbi</option>
            <option  value="/qemu/latest/source">qemu</option>
            <option  value="/toybox/latest/source">toybox</option>
            <option  value="/u-boot/latest/source">u-boot</option>
            <option  value="/uclibc-ng/latest/source">uclibc-ng</option>
            <option  value="/xen/latest/source">xen</option>
            <option  value="/zephyr/latest/source">zephyr</option>
        </select>
        <input class="filter-input" placeholder="Filter tags">
        <a title="Close Menu" class="close-menu icon-cross"></a>
        <button class="icon-filter" tabindex="-1"><span class="screenreader">Filter tags</span></button>
    </div>
    <nav>
        <h3 class="screenreader">Projects</h3>
        <ul class="projects">
            <li><a href="/amazon-freertos/latest/source">amazon-freertos</a></li>
            <li><a href="/arm-trusted-firmware/latest/source">arm-trusted-firmware</a></li>
            <li><a href="/barebox/latest/source">barebox</a></li>
            <li><a href="/bluez/latest/source">bluez</a></li>
            <li><a href="/busybox/latest/source">busybox</a></li>
            <li><a href="/coreboot/latest/source">coreboot</a></li>
            <li><a href="/dpdk/latest/source">dpdk</a></li>
            <li><a href="/freebsd/latest/source">freebsd</a></li>
            <li class="active" ><a href="/glibc/latest/source">glibc</a></li>
            <li><a href="/grub/latest/source">grub</a></li>
            <li><a href="/iproute2/latest/source">iproute2</a></li>
            <li><a href="/linux/latest/source">linux</a></li>
            <li><a href="/llvm/latest/source">llvm</a></li>
            <li><a href="/mesa/latest/source">mesa</a></li>
            <li><a href="/musl/latest/source">musl</a></li>
            <li><a href="/ofono/latest/source">ofono</a></li>
            <li><a href="/op-tee/latest/source">op-tee</a></li>
            <li><a href="/opensbi/latest/source">opensbi</a></li>
            <li><a href="/qemu/latest/source">qemu</a></li>
            <li><a href="/toybox/latest/source">toybox</a></li>
            <li><a href="/u-boot/latest/source">u-boot</a></li>
            <li><a href="/uclibc-ng/latest/source">uclibc-ng</a></li>
            <li><a href="/xen/latest/source">xen</a></li>
            <li><a href="/zephyr/latest/source">zephyr</a></li>
        </ul>

        <h3 class="screenreader">Versions</h3>
        <ul class="versions">
            
            
            <li>
                <span class="active">v2</span>
                <ul>
                
                    
                        <li>
                            <span class="active">v2.42</span>
                            <ul>
                                
                                    <li class="li-link active">
                                        <a href="/glibc/glibc-2.42.9000/source/stdlib/random_r.c">
                                            glibc-2.42.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.42/source/stdlib/random_r.c">
                                            glibc-2.42
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.41</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.41.9000/source/stdlib/random_r.c">
                                            glibc-2.41.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.41/source/stdlib/random_r.c">
                                            glibc-2.41
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.40</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.40.9000/source/stdlib/random_r.c">
                                            glibc-2.40.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.40/source/stdlib/random_r.c">
                                            glibc-2.40
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.39</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.39.9000/source/stdlib/random_r.c">
                                            glibc-2.39.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.39/source/stdlib/random_r.c">
                                            glibc-2.39
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.38</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.38.9000/source/stdlib/random_r.c">
                                            glibc-2.38.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.38/source/stdlib/random_r.c">
                                            glibc-2.38
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.37</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.37.9000/source/stdlib/random_r.c">
                                            glibc-2.37.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.37/source/stdlib/random_r.c">
                                            glibc-2.37
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.36</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.36.9000/source/stdlib/random_r.c">
                                            glibc-2.36.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.36/source/stdlib/random_r.c">
                                            glibc-2.36
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.35</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.35.9000/source/stdlib/random_r.c">
                                            glibc-2.35.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.35/source/stdlib/random_r.c">
                                            glibc-2.35
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.34</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.34.9000/source/stdlib/random_r.c">
                                            glibc-2.34.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.34/source/stdlib/random_r.c">
                                            glibc-2.34
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.33</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.33.9000/source/stdlib/random_r.c">
                                            glibc-2.33.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.33/source/stdlib/random_r.c">
                                            glibc-2.33
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.32</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.32.9000/source/stdlib/random_r.c">
                                            glibc-2.32.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.32/source/stdlib/random_r.c">
                                            glibc-2.32
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.31</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.31.9000/source/stdlib/random_r.c">
                                            glibc-2.31.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.31/source/stdlib/random_r.c">
                                            glibc-2.31
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.30</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.30.9000/source/stdlib/random_r.c">
                                            glibc-2.30.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.30/source/stdlib/random_r.c">
                                            glibc-2.30
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.29</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.29.9000/source/stdlib/random_r.c">
                                            glibc-2.29.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.29/source/stdlib/random_r.c">
                                            glibc-2.29
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.28</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.28.9000/source/stdlib/random_r.c">
                                            glibc-2.28.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.28/source/stdlib/random_r.c">
                                            glibc-2.28
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.27</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.27.9000/source/stdlib/random_r.c">
                                            glibc-2.27.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.27/source/stdlib/random_r.c">
                                            glibc-2.27
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.26</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.26.9000/source/stdlib/random_r.c">
                                            glibc-2.26.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.26/source/stdlib/random_r.c">
                                            glibc-2.26
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.25</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.25.90/source/stdlib/random_r.c">
                                            glibc-2.25.90
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.25/source/stdlib/random_r.c">
                                            glibc-2.25
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.24</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.24.90/source/stdlib/random_r.c">
                                            glibc-2.24.90
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.24/source/stdlib/random_r.c">
                                            glibc-2.24
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.23</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.23.90/source/stdlib/random_r.c">
                                            glibc-2.23.90
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.23/source/stdlib/random_r.c">
                                            glibc-2.23
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.22</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.22.90/source/stdlib/random_r.c">
                                            glibc-2.22.90
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.22/source/stdlib/random_r.c">
                                            glibc-2.22
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.21</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.21.90/source/stdlib/random_r.c">
                                            glibc-2.21.90
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.21/source/stdlib/random_r.c">
                                            glibc-2.21
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.20</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.20.90/source/stdlib/random_r.c">
                                            glibc-2.20.90
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.20/source/stdlib/random_r.c">
                                            glibc-2.20
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.19</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.19.90/source/stdlib/random_r.c">
                                            glibc-2.19.90
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.19/source/stdlib/random_r.c">
                                            glibc-2.19
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.18</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.18.90/source/stdlib/random_r.c">
                                            glibc-2.18.90
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.18/source/stdlib/random_r.c">
                                            glibc-2.18
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.17</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.17.90/source/stdlib/random_r.c">
                                            glibc-2.17.90
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.17/source/stdlib/random_r.c">
                                            glibc-2.17
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.16</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.16.90/source/stdlib/random_r.c">
                                            glibc-2.16.90
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.16.0/source/stdlib/random_r.c">
                                            glibc-2.16.0
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.16/source/stdlib/random_r.c">
                                            glibc-2.16
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.16-tps/source/stdlib/random_r.c">
                                            glibc-2.16-tps
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.16-ports-merge/source/stdlib/random_r.c">
                                            glibc-2.16-ports-merge
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.16-ports-before-merge/source/stdlib/random_r.c">
                                            glibc-2.16-ports-before-merge
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.15</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.15/source/stdlib/random_r.c">
                                            glibc-2.15
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.14</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.14.9000/source/stdlib/random_r.c">
                                            glibc-2.14.9000
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.14.1/source/stdlib/random_r.c">
                                            glibc-2.14.1
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.14/source/stdlib/random_r.c">
                                            glibc-2.14
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.13</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.13/source/stdlib/random_r.c">
                                            glibc-2.13
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.12</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.12.2/source/stdlib/random_r.c">
                                            glibc-2.12.2
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.12.1/source/stdlib/random_r.c">
                                            glibc-2.12.1
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.12/source/stdlib/random_r.c">
                                            glibc-2.12
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.11</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.11.3/source/stdlib/random_r.c">
                                            glibc-2.11.3
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.11.2/source/stdlib/random_r.c">
                                            glibc-2.11.2
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.11.1/source/stdlib/random_r.c">
                                            glibc-2.11.1
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.11/source/stdlib/random_r.c">
                                            glibc-2.11
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.10</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.10.2/source/stdlib/random_r.c">
                                            glibc-2.10.2
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.10.1/source/stdlib/random_r.c">
                                            glibc-2.10.1
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.10/source/stdlib/random_r.c">
                                            glibc-2.10
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.9</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.9/source/stdlib/random_r.c">
                                            glibc-2.9
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.8</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.8/source/stdlib/random_r.c">
                                            glibc-2.8
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.7</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.7/source/stdlib/random_r.c">
                                            glibc-2.7
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.6</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.6.1/source/stdlib/random_r.c">
                                            glibc-2.6.1
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.6/source/stdlib/random_r.c">
                                            glibc-2.6
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.5</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.5.1/source/stdlib/random_r.c">
                                            glibc-2.5.1
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.5/source/stdlib/random_r.c">
                                            glibc-2.5
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.4</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.4/source/stdlib/random_r.c">
                                            glibc-2.4
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.3</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.3.6/source/stdlib/random_r.c">
                                            glibc-2.3.6
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.3.5/source/stdlib/random_r.c">
                                            glibc-2.3.5
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.3.4/source/stdlib/random_r.c">
                                            glibc-2.3.4
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.3.3/source/stdlib/random_r.c">
                                            glibc-2.3.3
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.3.2/source/stdlib/random_r.c">
                                            glibc-2.3.2
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.3.1/source/stdlib/random_r.c">
                                            glibc-2.3.1
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.3/source/stdlib/random_r.c">
                                            glibc-2.3
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.2</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.2.5/source/stdlib/random_r.c">
                                            glibc-2.2.5
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.2.4/source/stdlib/random_r.c">
                                            glibc-2.2.4
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.2.3/source/stdlib/random_r.c">
                                            glibc-2.2.3
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.2.2/source/stdlib/random_r.c">
                                            glibc-2.2.2
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.2.1/source/stdlib/random_r.c">
                                            glibc-2.2.1
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.2/source/stdlib/random_r.c">
                                            glibc-2.2
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.1</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.1.97/source/stdlib/random_r.c">
                                            glibc-2.1.97
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.1.96/source/stdlib/random_r.c">
                                            glibc-2.1.96
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.1.95/source/stdlib/random_r.c">
                                            glibc-2.1.95
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.1.94/source/stdlib/random_r.c">
                                            glibc-2.1.94
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.1.93/source/stdlib/random_r.c">
                                            glibc-2.1.93
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.1.92/source/stdlib/random_r.c">
                                            glibc-2.1.92
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.1.91/source/stdlib/random_r.c">
                                            glibc-2.1.91
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.1.2/source/stdlib/random_r.c">
                                            glibc-2.1.2
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.1.1/source/stdlib/random_r.c">
                                            glibc-2.1.1
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.1/source/stdlib/random_r.c">
                                            glibc-2.1
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.0</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.0.112/source/stdlib/random_r.c">
                                            glibc-2.0.112
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.0.106/source/stdlib/random_r.c">
                                            glibc-2.0.106
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.0.103/source/stdlib/random_r.c">
                                            glibc-2.0.103
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.0.101/source/stdlib/random_r.c">
                                            glibc-2.0.101
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.0.100/source/stdlib/random_r.c">
                                            glibc-2.0.100
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.0.99/source/stdlib/random_r.c">
                                            glibc-2.0.99
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.0.98/source/stdlib/random_r.c">
                                            glibc-2.0.98
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.0.97/source/stdlib/random_r.c">
                                            glibc-2.0.97
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.0.96/source/stdlib/random_r.c">
                                            glibc-2.0.96
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.0.95/source/stdlib/random_r.c">
                                            glibc-2.0.95
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.0.92/source/stdlib/random_r.c">
                                            glibc-2.0.92
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.0.6/source/stdlib/random_r.c">
                                            glibc-2.0.6
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.0.5/source/stdlib/random_r.c">
                                            glibc-2.0.5
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.0.5b/source/stdlib/random_r.c">
                                            glibc-2.0.5b
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.0.4/source/stdlib/random_r.c">
                                            glibc-2.0.4
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-2.0.2/source/stdlib/random_r.c">
                                            glibc-2.0.2
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                </ul>
            </li>
        
            <li>
                <span class="">v1</span>
                <ul>
                
                    
                        <li>
                            <span class="">v1.93</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-1.93/source/stdlib/random_r.c">
                                            glibc-1.93
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v1.92</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-1.92/source/stdlib/random_r.c">
                                            glibc-1.92
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v1.91</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-1.91/source/stdlib/random_r.c">
                                            glibc-1.91
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v1.90</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-1.90/source/stdlib/random_r.c">
                                            glibc-1.90
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v1.09</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/glibc-1.09/source/stdlib/random_r.c">
                                            glibc-1.09
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                </ul>
            </li>
        
            <li>
                <span class="">fedora</span>
                <ul>
                
                    
                        <li>
                            <span class="">v2.12</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.12.90-6/source/stdlib/random_r.c">
                                            fedora/glibc-2.12.90-6
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.12.90-5/source/stdlib/random_r.c">
                                            fedora/glibc-2.12.90-5
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.12.90-4/source/stdlib/random_r.c">
                                            fedora/glibc-2.12.90-4
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.12.90-3/source/stdlib/random_r.c">
                                            fedora/glibc-2.12.90-3
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.12.90-2/source/stdlib/random_r.c">
                                            fedora/glibc-2.12.90-2
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.12.90-1/source/stdlib/random_r.c">
                                            fedora/glibc-2.12.90-1
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.12-3/source/stdlib/random_r.c">
                                            fedora/glibc-2.12-3
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.12-2/source/stdlib/random_r.c">
                                            fedora/glibc-2.12-2
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.12-1/source/stdlib/random_r.c">
                                            fedora/glibc-2.12-1
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.11</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-20/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-20
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-19/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-19
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-18/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-18
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-17/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-17
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-16/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-16
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-15/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-15
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-14/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-14
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-13/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-13
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-12/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-12
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-11/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-11
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-10/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-10
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-9/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-9
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-8/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-8
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-7/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-7
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-6/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-6
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-5/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-5
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-4/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-4
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-3/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-3
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-2/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-2
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.90-1/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.90-1
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.2-1/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.2-1
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.1-7/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.1-7
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.1-6/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.1-6
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.1-5/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.1-5
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.1-4/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.1-4
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11.1-1/source/stdlib/random_r.c">
                                            fedora/glibc-2.11.1-1
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11-4/source/stdlib/random_r.c">
                                            fedora/glibc-2.11-4
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11-3/source/stdlib/random_r.c">
                                            fedora/glibc-2.11-3
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11-2/source/stdlib/random_r.c">
                                            fedora/glibc-2.11-2
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.11-1/source/stdlib/random_r.c">
                                            fedora/glibc-2.11-1
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                    
                        <li>
                            <span class="">v2.10</span>
                            <ul>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-27/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-27
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-26/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-26
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-25/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-25
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-24/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-24
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-23/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-23
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-22/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-22
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-21/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-21
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-20/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-20
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-19/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-19
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-18/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-18
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-17/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-17
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-16/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-16
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-15/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-15
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-14/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-14
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-13/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-13
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-12/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-12
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-11/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-11
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-10/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-10
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-9/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-9
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-7/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-7
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-6/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-6
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-5/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-5
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-4/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-4
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-3/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-3
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-2/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-2
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.90-1/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.90-1
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.2-1/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.2-1
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.1-5/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.1-5
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.1-4/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.1-4
                                        </a>
                                    </li>
                                
                                    <li class="li-link ">
                                        <a href="/glibc/fedora%2Fglibc-2.10.1-3/source/stdlib/random_r.c">
                                            fedora/glibc-2.10.1-3
                                        </a>
                                    </li>
                                
                            </ul>
                        </li>
                    
                
                </ul>
            </li>
        
        </ul>

        <div class="filter-results"></div>
    </nav>
</aside>
                <div class="workspace">
                    <div id="reference-popup-wrapper">
                        <div id="reference-popup"></div>
                        <div id="loading-popup">Loading...</div>
                    </div>
                    
<div class="lxrcode">
    <div class="highlight"><table class="highlighttable"><tr><td class="linenos"><div class="linenodiv"><pre><span class="normal"><a name="L1" id="L1" href="#L1">  1</a></span>
<span class="normal"><a name="L2" id="L2" href="#L2">  2</a></span>
<span class="normal"><a name="L3" id="L3" href="#L3">  3</a></span>
<span class="normal"><a name="L4" id="L4" href="#L4">  4</a></span>
<span class="normal"><a name="L5" id="L5" href="#L5">  5</a></span>
<span class="normal"><a name="L6" id="L6" href="#L6">  6</a></span>
<span class="normal"><a name="L7" id="L7" href="#L7">  7</a></span>
<span class="normal"><a name="L8" id="L8" href="#L8">  8</a></span>
<span class="normal"><a name="L9" id="L9" href="#L9">  9</a></span>
<span class="normal"><a name="L10" id="L10" href="#L10"> 10</a></span>
<span class="normal"><a name="L11" id="L11" href="#L11"> 11</a></span>
<span class="normal"><a name="L12" id="L12" href="#L12"> 12</a></span>
<span class="normal"><a name="L13" id="L13" href="#L13"> 13</a></span>
<span class="normal"><a name="L14" id="L14" href="#L14"> 14</a></span>
<span class="normal"><a name="L15" id="L15" href="#L15"> 15</a></span>
<span class="normal"><a name="L16" id="L16" href="#L16"> 16</a></span>
<span class="normal"><a name="L17" id="L17" href="#L17"> 17</a></span>
<span class="normal"><a name="L18" id="L18" href="#L18"> 18</a></span>
<span class="normal"><a name="L19" id="L19" href="#L19"> 19</a></span>
<span class="normal"><a name="L20" id="L20" href="#L20"> 20</a></span>
<span class="normal"><a name="L21" id="L21" href="#L21"> 21</a></span>
<span class="normal"><a name="L22" id="L22" href="#L22"> 22</a></span>
<span class="normal"><a name="L23" id="L23" href="#L23"> 23</a></span>
<span class="normal"><a name="L24" id="L24" href="#L24"> 24</a></span>
<span class="normal"><a name="L25" id="L25" href="#L25"> 25</a></span>
<span class="normal"><a name="L26" id="L26" href="#L26"> 26</a></span>
<span class="normal"><a name="L27" id="L27" href="#L27"> 27</a></span>
<span class="normal"><a name="L28" id="L28" href="#L28"> 28</a></span>
<span class="normal"><a name="L29" id="L29" href="#L29"> 29</a></span>
<span class="normal"><a name="L30" id="L30" href="#L30"> 30</a></span>
<span class="normal"><a name="L31" id="L31" href="#L31"> 31</a></span>
<span class="normal"><a name="L32" id="L32" href="#L32"> 32</a></span>
<span class="normal"><a name="L33" id="L33" href="#L33"> 33</a></span>
<span class="normal"><a name="L34" id="L34" href="#L34"> 34</a></span>
<span class="normal"><a name="L35" id="L35" href="#L35"> 35</a></span>
<span class="normal"><a name="L36" id="L36" href="#L36"> 36</a></span>
<span class="normal"><a name="L37" id="L37" href="#L37"> 37</a></span>
<span class="normal"><a name="L38" id="L38" href="#L38"> 38</a></span>
<span class="normal"><a name="L39" id="L39" href="#L39"> 39</a></span>
<span class="normal"><a name="L40" id="L40" href="#L40"> 40</a></span>
<span class="normal"><a name="L41" id="L41" href="#L41"> 41</a></span>
<span class="normal"><a name="L42" id="L42" href="#L42"> 42</a></span>
<span class="normal"><a name="L43" id="L43" href="#L43"> 43</a></span>
<span class="normal"><a name="L44" id="L44" href="#L44"> 44</a></span>
<span class="normal"><a name="L45" id="L45" href="#L45"> 45</a></span>
<span class="normal"><a name="L46" id="L46" href="#L46"> 46</a></span>
<span class="normal"><a name="L47" id="L47" href="#L47"> 47</a></span>
<span class="normal"><a name="L48" id="L48" href="#L48"> 48</a></span>
<span class="normal"><a name="L49" id="L49" href="#L49"> 49</a></span>
<span class="normal"><a name="L50" id="L50" href="#L50"> 50</a></span>
<span class="normal"><a name="L51" id="L51" href="#L51"> 51</a></span>
<span class="normal"><a name="L52" id="L52" href="#L52"> 52</a></span>
<span class="normal"><a name="L53" id="L53" href="#L53"> 53</a></span>
<span class="normal"><a name="L54" id="L54" href="#L54"> 54</a></span>
<span class="normal"><a name="L55" id="L55" href="#L55"> 55</a></span>
<span class="normal"><a name="L56" id="L56" href="#L56"> 56</a></span>
<span class="normal"><a name="L57" id="L57" href="#L57"> 57</a></span>
<span class="normal"><a name="L58" id="L58" href="#L58"> 58</a></span>
<span class="normal"><a name="L59" id="L59" href="#L59"> 59</a></span>
<span class="normal"><a name="L60" id="L60" href="#L60"> 60</a></span>
<span class="normal"><a name="L61" id="L61" href="#L61"> 61</a></span>
<span class="normal"><a name="L62" id="L62" href="#L62"> 62</a></span>
<span class="normal"><a name="L63" id="L63" href="#L63"> 63</a></span>
<span class="normal"><a name="L64" id="L64" href="#L64"> 64</a></span>
<span class="normal"><a name="L65" id="L65" href="#L65"> 65</a></span>
<span class="normal"><a name="L66" id="L66" href="#L66"> 66</a></span>
<span class="normal"><a name="L67" id="L67" href="#L67"> 67</a></span>
<span class="normal"><a name="L68" id="L68" href="#L68"> 68</a></span>
<span class="normal"><a name="L69" id="L69" href="#L69"> 69</a></span>
<span class="normal"><a name="L70" id="L70" href="#L70"> 70</a></span>
<span class="normal"><a name="L71" id="L71" href="#L71"> 71</a></span>
<span class="normal"><a name="L72" id="L72" href="#L72"> 72</a></span>
<span class="normal"><a name="L73" id="L73" href="#L73"> 73</a></span>
<span class="normal"><a name="L74" id="L74" href="#L74"> 74</a></span>
<span class="normal"><a name="L75" id="L75" href="#L75"> 75</a></span>
<span class="normal"><a name="L76" id="L76" href="#L76"> 76</a></span>
<span class="normal"><a name="L77" id="L77" href="#L77"> 77</a></span>
<span class="normal"><a name="L78" id="L78" href="#L78"> 78</a></span>
<span class="normal"><a name="L79" id="L79" href="#L79"> 79</a></span>
<span class="normal"><a name="L80" id="L80" href="#L80"> 80</a></span>
<span class="normal"><a name="L81" id="L81" href="#L81"> 81</a></span>
<span class="normal"><a name="L82" id="L82" href="#L82"> 82</a></span>
<span class="normal"><a name="L83" id="L83" href="#L83"> 83</a></span>
<span class="normal"><a name="L84" id="L84" href="#L84"> 84</a></span>
<span class="normal"><a name="L85" id="L85" href="#L85"> 85</a></span>
<span class="normal"><a name="L86" id="L86" href="#L86"> 86</a></span>
<span class="normal"><a name="L87" id="L87" href="#L87"> 87</a></span>
<span class="normal"><a name="L88" id="L88" href="#L88"> 88</a></span>
<span class="normal"><a name="L89" id="L89" href="#L89"> 89</a></span>
<span class="normal"><a name="L90" id="L90" href="#L90"> 90</a></span>
<span class="normal"><a name="L91" id="L91" href="#L91"> 91</a></span>
<span class="normal"><a name="L92" id="L92" href="#L92"> 92</a></span>
<span class="normal"><a name="L93" id="L93" href="#L93"> 93</a></span>
<span class="normal"><a name="L94" id="L94" href="#L94"> 94</a></span>
<span class="normal"><a name="L95" id="L95" href="#L95"> 95</a></span>
<span class="normal"><a name="L96" id="L96" href="#L96"> 96</a></span>
<span class="normal"><a name="L97" id="L97" href="#L97"> 97</a></span>
<span class="normal"><a name="L98" id="L98" href="#L98"> 98</a></span>
<span class="normal"><a name="L99" id="L99" href="#L99"> 99</a></span>
<span class="normal"><a name="L100" id="L100" href="#L100">100</a></span>
<span class="normal"><a name="L101" id="L101" href="#L101">101</a></span>
<span class="normal"><a name="L102" id="L102" href="#L102">102</a></span>
<span class="normal"><a name="L103" id="L103" href="#L103">103</a></span>
<span class="normal"><a name="L104" id="L104" href="#L104">104</a></span>
<span class="normal"><a name="L105" id="L105" href="#L105">105</a></span>
<span class="normal"><a name="L106" id="L106" href="#L106">106</a></span>
<span class="normal"><a name="L107" id="L107" href="#L107">107</a></span>
<span class="normal"><a name="L108" id="L108" href="#L108">108</a></span>
<span class="normal"><a name="L109" id="L109" href="#L109">109</a></span>
<span class="normal"><a name="L110" id="L110" href="#L110">110</a></span>
<span class="normal"><a name="L111" id="L111" href="#L111">111</a></span>
<span class="normal"><a name="L112" id="L112" href="#L112">112</a></span>
<span class="normal"><a name="L113" id="L113" href="#L113">113</a></span>
<span class="normal"><a name="L114" id="L114" href="#L114">114</a></span>
<span class="normal"><a name="L115" id="L115" href="#L115">115</a></span>
<span class="normal"><a name="L116" id="L116" href="#L116">116</a></span>
<span class="normal"><a name="L117" id="L117" href="#L117">117</a></span>
<span class="normal"><a name="L118" id="L118" href="#L118">118</a></span>
<span class="normal"><a name="L119" id="L119" href="#L119">119</a></span>
<span class="normal"><a name="L120" id="L120" href="#L120">120</a></span>
<span class="normal"><a name="L121" id="L121" href="#L121">121</a></span>
<span class="normal"><a name="L122" id="L122" href="#L122">122</a></span>
<span class="normal"><a name="L123" id="L123" href="#L123">123</a></span>
<span class="normal"><a name="L124" id="L124" href="#L124">124</a></span>
<span class="normal"><a name="L125" id="L125" href="#L125">125</a></span>
<span class="normal"><a name="L126" id="L126" href="#L126">126</a></span>
<span class="normal"><a name="L127" id="L127" href="#L127">127</a></span>
<span class="normal"><a name="L128" id="L128" href="#L128">128</a></span>
<span class="normal"><a name="L129" id="L129" href="#L129">129</a></span>
<span class="normal"><a name="L130" id="L130" href="#L130">130</a></span>
<span class="normal"><a name="L131" id="L131" href="#L131">131</a></span>
<span class="normal"><a name="L132" id="L132" href="#L132">132</a></span>
<span class="normal"><a name="L133" id="L133" href="#L133">133</a></span>
<span class="normal"><a name="L134" id="L134" href="#L134">134</a></span>
<span class="normal"><a name="L135" id="L135" href="#L135">135</a></span>
<span class="normal"><a name="L136" id="L136" href="#L136">136</a></span>
<span class="normal"><a name="L137" id="L137" href="#L137">137</a></span>
<span class="normal"><a name="L138" id="L138" href="#L138">138</a></span>
<span class="normal"><a name="L139" id="L139" href="#L139">139</a></span>
<span class="normal"><a name="L140" id="L140" href="#L140">140</a></span>
<span class="normal"><a name="L141" id="L141" href="#L141">141</a></span>
<span class="normal"><a name="L142" id="L142" href="#L142">142</a></span>
<span class="normal"><a name="L143" id="L143" href="#L143">143</a></span>
<span class="normal"><a name="L144" id="L144" href="#L144">144</a></span>
<span class="normal"><a name="L145" id="L145" href="#L145">145</a></span>
<span class="normal"><a name="L146" id="L146" href="#L146">146</a></span>
<span class="normal"><a name="L147" id="L147" href="#L147">147</a></span>
<span class="normal"><a name="L148" id="L148" href="#L148">148</a></span>
<span class="normal"><a name="L149" id="L149" href="#L149">149</a></span>
<span class="normal"><a name="L150" id="L150" href="#L150">150</a></span>
<span class="normal"><a name="L151" id="L151" href="#L151">151</a></span>
<span class="normal"><a name="L152" id="L152" href="#L152">152</a></span>
<span class="normal"><a name="L153" id="L153" href="#L153">153</a></span>
<span class="normal"><a name="L154" id="L154" href="#L154">154</a></span>
<span class="normal"><a name="L155" id="L155" href="#L155">155</a></span>
<span class="normal"><a name="L156" id="L156" href="#L156">156</a></span>
<span class="normal"><a name="L157" id="L157" href="#L157">157</a></span>
<span class="normal"><a name="L158" id="L158" href="#L158">158</a></span>
<span class="normal"><a name="L159" id="L159" href="#L159">159</a></span>
<span class="normal"><a name="L160" id="L160" href="#L160">160</a></span>
<span class="normal"><a name="L161" id="L161" href="#L161">161</a></span>
<span class="normal"><a name="L162" id="L162" href="#L162">162</a></span>
<span class="normal"><a name="L163" id="L163" href="#L163">163</a></span>
<span class="normal"><a name="L164" id="L164" href="#L164">164</a></span>
<span class="normal"><a name="L165" id="L165" href="#L165">165</a></span>
<span class="normal"><a name="L166" id="L166" href="#L166">166</a></span>
<span class="normal"><a name="L167" id="L167" href="#L167">167</a></span>
<span class="normal"><a name="L168" id="L168" href="#L168">168</a></span>
<span class="normal"><a name="L169" id="L169" href="#L169">169</a></span>
<span class="normal"><a name="L170" id="L170" href="#L170">170</a></span>
<span class="normal"><a name="L171" id="L171" href="#L171">171</a></span>
<span class="normal"><a name="L172" id="L172" href="#L172">172</a></span>
<span class="normal"><a name="L173" id="L173" href="#L173">173</a></span>
<span class="normal"><a name="L174" id="L174" href="#L174">174</a></span>
<span class="normal"><a name="L175" id="L175" href="#L175">175</a></span>
<span class="normal"><a name="L176" id="L176" href="#L176">176</a></span>
<span class="normal"><a name="L177" id="L177" href="#L177">177</a></span>
<span class="normal"><a name="L178" id="L178" href="#L178">178</a></span>
<span class="normal"><a name="L179" id="L179" href="#L179">179</a></span>
<span class="normal"><a name="L180" id="L180" href="#L180">180</a></span>
<span class="normal"><a name="L181" id="L181" href="#L181">181</a></span>
<span class="normal"><a name="L182" id="L182" href="#L182">182</a></span>
<span class="normal"><a name="L183" id="L183" href="#L183">183</a></span>
<span class="normal"><a name="L184" id="L184" href="#L184">184</a></span>
<span class="normal"><a name="L185" id="L185" href="#L185">185</a></span>
<span class="normal"><a name="L186" id="L186" href="#L186">186</a></span>
<span class="normal"><a name="L187" id="L187" href="#L187">187</a></span>
<span class="normal"><a name="L188" id="L188" href="#L188">188</a></span>
<span class="normal"><a name="L189" id="L189" href="#L189">189</a></span>
<span class="normal"><a name="L190" id="L190" href="#L190">190</a></span>
<span class="normal"><a name="L191" id="L191" href="#L191">191</a></span>
<span class="normal"><a name="L192" id="L192" href="#L192">192</a></span>
<span class="normal"><a name="L193" id="L193" href="#L193">193</a></span>
<span class="normal"><a name="L194" id="L194" href="#L194">194</a></span>
<span class="normal"><a name="L195" id="L195" href="#L195">195</a></span>
<span class="normal"><a name="L196" id="L196" href="#L196">196</a></span>
<span class="normal"><a name="L197" id="L197" href="#L197">197</a></span>
<span class="normal"><a name="L198" id="L198" href="#L198">198</a></span>
<span class="normal"><a name="L199" id="L199" href="#L199">199</a></span>
<span class="normal"><a name="L200" id="L200" href="#L200">200</a></span>
<span class="normal"><a name="L201" id="L201" href="#L201">201</a></span>
<span class="normal"><a name="L202" id="L202" href="#L202">202</a></span>
<span class="normal"><a name="L203" id="L203" href="#L203">203</a></span>
<span class="normal"><a name="L204" id="L204" href="#L204">204</a></span>
<span class="normal"><a name="L205" id="L205" href="#L205">205</a></span>
<span class="normal"><a name="L206" id="L206" href="#L206">206</a></span>
<span class="normal"><a name="L207" id="L207" href="#L207">207</a></span>
<span class="normal"><a name="L208" id="L208" href="#L208">208</a></span>
<span class="normal"><a name="L209" id="L209" href="#L209">209</a></span>
<span class="normal"><a name="L210" id="L210" href="#L210">210</a></span>
<span class="normal"><a name="L211" id="L211" href="#L211">211</a></span>
<span class="normal"><a name="L212" id="L212" href="#L212">212</a></span>
<span class="normal"><a name="L213" id="L213" href="#L213">213</a></span>
<span class="normal"><a name="L214" id="L214" href="#L214">214</a></span>
<span class="normal"><a name="L215" id="L215" href="#L215">215</a></span>
<span class="normal"><a name="L216" id="L216" href="#L216">216</a></span>
<span class="normal"><a name="L217" id="L217" href="#L217">217</a></span>
<span class="normal"><a name="L218" id="L218" href="#L218">218</a></span>
<span class="normal"><a name="L219" id="L219" href="#L219">219</a></span>
<span class="normal"><a name="L220" id="L220" href="#L220">220</a></span>
<span class="normal"><a name="L221" id="L221" href="#L221">221</a></span>
<span class="normal"><a name="L222" id="L222" href="#L222">222</a></span>
<span class="normal"><a name="L223" id="L223" href="#L223">223</a></span>
<span class="normal"><a name="L224" id="L224" href="#L224">224</a></span>
<span class="normal"><a name="L225" id="L225" href="#L225">225</a></span>
<span class="normal"><a name="L226" id="L226" href="#L226">226</a></span>
<span class="normal"><a name="L227" id="L227" href="#L227">227</a></span>
<span class="normal"><a name="L228" id="L228" href="#L228">228</a></span>
<span class="normal"><a name="L229" id="L229" href="#L229">229</a></span>
<span class="normal"><a name="L230" id="L230" href="#L230">230</a></span>
<span class="normal"><a name="L231" id="L231" href="#L231">231</a></span>
<span class="normal"><a name="L232" id="L232" href="#L232">232</a></span>
<span class="normal"><a name="L233" id="L233" href="#L233">233</a></span>
<span class="normal"><a name="L234" id="L234" href="#L234">234</a></span>
<span class="normal"><a name="L235" id="L235" href="#L235">235</a></span>
<span class="normal"><a name="L236" id="L236" href="#L236">236</a></span>
<span class="normal"><a name="L237" id="L237" href="#L237">237</a></span>
<span class="normal"><a name="L238" id="L238" href="#L238">238</a></span>
<span class="normal"><a name="L239" id="L239" href="#L239">239</a></span>
<span class="normal"><a name="L240" id="L240" href="#L240">240</a></span>
<span class="normal"><a name="L241" id="L241" href="#L241">241</a></span>
<span class="normal"><a name="L242" id="L242" href="#L242">242</a></span>
<span class="normal"><a name="L243" id="L243" href="#L243">243</a></span>
<span class="normal"><a name="L244" id="L244" href="#L244">244</a></span>
<span class="normal"><a name="L245" id="L245" href="#L245">245</a></span>
<span class="normal"><a name="L246" id="L246" href="#L246">246</a></span>
<span class="normal"><a name="L247" id="L247" href="#L247">247</a></span>
<span class="normal"><a name="L248" id="L248" href="#L248">248</a></span>
<span class="normal"><a name="L249" id="L249" href="#L249">249</a></span>
<span class="normal"><a name="L250" id="L250" href="#L250">250</a></span>
<span class="normal"><a name="L251" id="L251" href="#L251">251</a></span>
<span class="normal"><a name="L252" id="L252" href="#L252">252</a></span>
<span class="normal"><a name="L253" id="L253" href="#L253">253</a></span>
<span class="normal"><a name="L254" id="L254" href="#L254">254</a></span>
<span class="normal"><a name="L255" id="L255" href="#L255">255</a></span>
<span class="normal"><a name="L256" id="L256" href="#L256">256</a></span>
<span class="normal"><a name="L257" id="L257" href="#L257">257</a></span>
<span class="normal"><a name="L258" id="L258" href="#L258">258</a></span>
<span class="normal"><a name="L259" id="L259" href="#L259">259</a></span>
<span class="normal"><a name="L260" id="L260" href="#L260">260</a></span>
<span class="normal"><a name="L261" id="L261" href="#L261">261</a></span>
<span class="normal"><a name="L262" id="L262" href="#L262">262</a></span>
<span class="normal"><a name="L263" id="L263" href="#L263">263</a></span>
<span class="normal"><a name="L264" id="L264" href="#L264">264</a></span>
<span class="normal"><a name="L265" id="L265" href="#L265">265</a></span>
<span class="normal"><a name="L266" id="L266" href="#L266">266</a></span>
<span class="normal"><a name="L267" id="L267" href="#L267">267</a></span>
<span class="normal"><a name="L268" id="L268" href="#L268">268</a></span>
<span class="normal"><a name="L269" id="L269" href="#L269">269</a></span>
<span class="normal"><a name="L270" id="L270" href="#L270">270</a></span>
<span class="normal"><a name="L271" id="L271" href="#L271">271</a></span>
<span class="normal"><a name="L272" id="L272" href="#L272">272</a></span>
<span class="normal"><a name="L273" id="L273" href="#L273">273</a></span>
<span class="normal"><a name="L274" id="L274" href="#L274">274</a></span>
<span class="normal"><a name="L275" id="L275" href="#L275">275</a></span>
<span class="normal"><a name="L276" id="L276" href="#L276">276</a></span>
<span class="normal"><a name="L277" id="L277" href="#L277">277</a></span>
<span class="normal"><a name="L278" id="L278" href="#L278">278</a></span>
<span class="normal"><a name="L279" id="L279" href="#L279">279</a></span>
<span class="normal"><a name="L280" id="L280" href="#L280">280</a></span>
<span class="normal"><a name="L281" id="L281" href="#L281">281</a></span>
<span class="normal"><a name="L282" id="L282" href="#L282">282</a></span>
<span class="normal"><a name="L283" id="L283" href="#L283">283</a></span>
<span class="normal"><a name="L284" id="L284" href="#L284">284</a></span>
<span class="normal"><a name="L285" id="L285" href="#L285">285</a></span>
<span class="normal"><a name="L286" id="L286" href="#L286">286</a></span>
<span class="normal"><a name="L287" id="L287" href="#L287">287</a></span>
<span class="normal"><a name="L288" id="L288" href="#L288">288</a></span>
<span class="normal"><a name="L289" id="L289" href="#L289">289</a></span>
<span class="normal"><a name="L290" id="L290" href="#L290">290</a></span>
<span class="normal"><a name="L291" id="L291" href="#L291">291</a></span>
<span class="normal"><a name="L292" id="L292" href="#L292">292</a></span>
<span class="normal"><a name="L293" id="L293" href="#L293">293</a></span>
<span class="normal"><a name="L294" id="L294" href="#L294">294</a></span>
<span class="normal"><a name="L295" id="L295" href="#L295">295</a></span>
<span class="normal"><a name="L296" id="L296" href="#L296">296</a></span>
<span class="normal"><a name="L297" id="L297" href="#L297">297</a></span>
<span class="normal"><a name="L298" id="L298" href="#L298">298</a></span>
<span class="normal"><a name="L299" id="L299" href="#L299">299</a></span>
<span class="normal"><a name="L300" id="L300" href="#L300">300</a></span>
<span class="normal"><a name="L301" id="L301" href="#L301">301</a></span>
<span class="normal"><a name="L302" id="L302" href="#L302">302</a></span>
<span class="normal"><a name="L303" id="L303" href="#L303">303</a></span>
<span class="normal"><a name="L304" id="L304" href="#L304">304</a></span>
<span class="normal"><a name="L305" id="L305" href="#L305">305</a></span>
<span class="normal"><a name="L306" id="L306" href="#L306">306</a></span>
<span class="normal"><a name="L307" id="L307" href="#L307">307</a></span>
<span class="normal"><a name="L308" id="L308" href="#L308">308</a></span>
<span class="normal"><a name="L309" id="L309" href="#L309">309</a></span>
<span class="normal"><a name="L310" id="L310" href="#L310">310</a></span>
<span class="normal"><a name="L311" id="L311" href="#L311">311</a></span>
<span class="normal"><a name="L312" id="L312" href="#L312">312</a></span>
<span class="normal"><a name="L313" id="L313" href="#L313">313</a></span>
<span class="normal"><a name="L314" id="L314" href="#L314">314</a></span>
<span class="normal"><a name="L315" id="L315" href="#L315">315</a></span>
<span class="normal"><a name="L316" id="L316" href="#L316">316</a></span>
<span class="normal"><a name="L317" id="L317" href="#L317">317</a></span>
<span class="normal"><a name="L318" id="L318" href="#L318">318</a></span>
<span class="normal"><a name="L319" id="L319" href="#L319">319</a></span>
<span class="normal"><a name="L320" id="L320" href="#L320">320</a></span>
<span class="normal"><a name="L321" id="L321" href="#L321">321</a></span>
<span class="normal"><a name="L322" id="L322" href="#L322">322</a></span>
<span class="normal"><a name="L323" id="L323" href="#L323">323</a></span>
<span class="normal"><a name="L324" id="L324" href="#L324">324</a></span>
<span class="normal"><a name="L325" id="L325" href="#L325">325</a></span>
<span class="normal"><a name="L326" id="L326" href="#L326">326</a></span>
<span class="normal"><a name="L327" id="L327" href="#L327">327</a></span>
<span class="normal"><a name="L328" id="L328" href="#L328">328</a></span>
<span class="normal"><a name="L329" id="L329" href="#L329">329</a></span>
<span class="normal"><a name="L330" id="L330" href="#L330">330</a></span>
<span class="normal"><a name="L331" id="L331" href="#L331">331</a></span>
<span class="normal"><a name="L332" id="L332" href="#L332">332</a></span>
<span class="normal"><a name="L333" id="L333" href="#L333">333</a></span>
<span class="normal"><a name="L334" id="L334" href="#L334">334</a></span>
<span class="normal"><a name="L335" id="L335" href="#L335">335</a></span>
<span class="normal"><a name="L336" id="L336" href="#L336">336</a></span>
<span class="normal"><a name="L337" id="L337" href="#L337">337</a></span>
<span class="normal"><a name="L338" id="L338" href="#L338">338</a></span>
<span class="normal"><a name="L339" id="L339" href="#L339">339</a></span>
<span class="normal"><a name="L340" id="L340" href="#L340">340</a></span>
<span class="normal"><a name="L341" id="L341" href="#L341">341</a></span>
<span class="normal"><a name="L342" id="L342" href="#L342">342</a></span>
<span class="normal"><a name="L343" id="L343" href="#L343">343</a></span>
<span class="normal"><a name="L344" id="L344" href="#L344">344</a></span>
<span class="normal"><a name="L345" id="L345" href="#L345">345</a></span>
<span class="normal"><a name="L346" id="L346" href="#L346">346</a></span>
<span class="normal"><a name="L347" id="L347" href="#L347">347</a></span>
<span class="normal"><a name="L348" id="L348" href="#L348">348</a></span>
<span class="normal"><a name="L349" id="L349" href="#L349">349</a></span>
<span class="normal"><a name="L350" id="L350" href="#L350">350</a></span>
<span class="normal"><a name="L351" id="L351" href="#L351">351</a></span>
<span class="normal"><a name="L352" id="L352" href="#L352">352</a></span>
<span class="normal"><a name="L353" id="L353" href="#L353">353</a></span>
<span class="normal"><a name="L354" id="L354" href="#L354">354</a></span>
<span class="normal"><a name="L355" id="L355" href="#L355">355</a></span>
<span class="normal"><a name="L356" id="L356" href="#L356">356</a></span>
<span class="normal"><a name="L357" id="L357" href="#L357">357</a></span>
<span class="normal"><a name="L358" id="L358" href="#L358">358</a></span>
<span class="normal"><a name="L359" id="L359" href="#L359">359</a></span>
<span class="normal"><a name="L360" id="L360" href="#L360">360</a></span>
<span class="normal"><a name="L361" id="L361" href="#L361">361</a></span>
<span class="normal"><a name="L362" id="L362" href="#L362">362</a></span>
<span class="normal"><a name="L363" id="L363" href="#L363">363</a></span>
<span class="normal"><a name="L364" id="L364" href="#L364">364</a></span>
<span class="normal"><a name="L365" id="L365" href="#L365">365</a></span>
<span class="normal"><a name="L366" id="L366" href="#L366">366</a></span>
<span class="normal"><a name="L367" id="L367" href="#L367">367</a></span>
<span class="normal"><a name="L368" id="L368" href="#L368">368</a></span>
<span class="normal"><a name="L369" id="L369" href="#L369">369</a></span>
<span class="normal"><a name="L370" id="L370" href="#L370">370</a></span>
<span class="normal"><a name="L371" id="L371" href="#L371">371</a></span>
<span class="normal"><a name="L372" id="L372" href="#L372">372</a></span>
<span class="normal"><a name="L373" id="L373" href="#L373">373</a></span>
<span class="normal"><a name="L374" id="L374" href="#L374">374</a></span>
<span class="normal"><a name="L375" id="L375" href="#L375">375</a></span>
<span class="normal"><a name="L376" id="L376" href="#L376">376</a></span>
<span class="normal"><a name="L377" id="L377" href="#L377">377</a></span>
<span class="normal"><a name="L378" id="L378" href="#L378">378</a></span>
<span class="normal"><a name="L379" id="L379" href="#L379">379</a></span>
<span class="normal"><a name="L380" id="L380" href="#L380">380</a></span>
<span class="normal"><a name="L381" id="L381" href="#L381">381</a></span>
<span class="normal"><a name="L382" id="L382" href="#L382">382</a></span>
<span class="normal"><a name="L383" id="L383" href="#L383">383</a></span>
<span class="normal"><a name="L384" id="L384" href="#L384">384</a></span>
<span class="normal"><a name="L385" id="L385" href="#L385">385</a></span>
<span class="normal"><a name="L386" id="L386" href="#L386">386</a></span>
<span class="normal"><a name="L387" id="L387" href="#L387">387</a></span>
<span class="normal"><a name="L388" id="L388" href="#L388">388</a></span>
<span class="normal"><a name="L389" id="L389" href="#L389">389</a></span>
<span class="normal"><a name="L390" id="L390" href="#L390">390</a></span>
<span class="normal"><a name="L391" id="L391" href="#L391">391</a></span>
<span class="normal"><a name="L392" id="L392" href="#L392">392</a></span>
<span class="normal"><a name="L393" id="L393" href="#L393">393</a></span>
<span class="normal"><a name="L394" id="L394" href="#L394">394</a></span>
<span class="normal"><a name="L395" id="L395" href="#L395">395</a></span>
<span class="normal"><a name="L396" id="L396" href="#L396">396</a></span>
<span class="normal"><a name="L397" id="L397" href="#L397">397</a></span>
<span class="normal"><a name="L398" id="L398" href="#L398">398</a></span>
<span class="normal"><a name="L399" id="L399" href="#L399">399</a></span>
<span class="normal"><a name="L400" id="L400" href="#L400">400</a></span>
<span class="normal"><a name="L401" id="L401" href="#L401">401</a></span>
<span class="normal"><a name="L402" id="L402" href="#L402">402</a></span>
<span class="normal"><a name="L403" id="L403" href="#L403">403</a></span>
<span class="normal"><a name="L404" id="L404" href="#L404">404</a></span>
<span class="normal"><a name="L405" id="L405" href="#L405">405</a></span>
<span class="normal"><a name="L406" id="L406" href="#L406">406</a></span>
<span class="normal"><a name="L407" id="L407" href="#L407">407</a></span>
<span class="normal"><a name="L408" id="L408" href="#L408">408</a></span>
<span class="normal"><a name="L409" id="L409" href="#L409">409</a></span>
<span class="normal"><a name="L410" id="L410" href="#L410">410</a></span>
<span class="normal"><a name="L411" id="L411" href="#L411">411</a></span>
<span class="normal"><a name="L412" id="L412" href="#L412">412</a></span>
<span class="normal"><a name="L413" id="L413" href="#L413">413</a></span>
<span class="normal"><a name="L414" id="L414" href="#L414">414</a></span>
<span class="normal"><a name="L415" id="L415" href="#L415">415</a></span>
<span class="normal"><a name="L416" id="L416" href="#L416">416</a></span>
<span class="normal"><a name="L417" id="L417" href="#L417">417</a></span>
<span class="normal"><a name="L418" id="L418" href="#L418">418</a></span>
<span class="normal"><a name="L419" id="L419" href="#L419">419</a></span>
<span class="normal"><a name="L420" id="L420" href="#L420">420</a></span>
<span class="normal"><a name="L421" id="L421" href="#L421">421</a></span></pre></div></td><td class="code"><div><pre><span></span><span id="codeline-1"><span class="cm">/*</span>
</span><span id="codeline-2"><span class="cm">   Copyright (C) 1995-2025 Free Software Foundation, Inc.</span>
</span><span id="codeline-3">
</span><span id="codeline-4"><span class="cm">   The GNU C Library is free software; you can redistribute it and/or</span>
</span><span id="codeline-5"><span class="cm">   modify it under the terms of the GNU Lesser General Public</span>
</span><span id="codeline-6"><span class="cm">   License as published by the Free Software Foundation; either</span>
</span><span id="codeline-7"><span class="cm">   version 2.1 of the License, or (at your option) any later version.</span>
</span><span id="codeline-8">
</span><span id="codeline-9"><span class="cm">   The GNU C Library is distributed in the hope that it will be useful,</span>
</span><span id="codeline-10"><span class="cm">   but WITHOUT ANY WARRANTY; without even the implied warranty of</span>
</span><span id="codeline-11"><span class="cm">   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU</span>
</span><span id="codeline-12"><span class="cm">   Lesser General Public License for more details.</span>
</span><span id="codeline-13">
</span><span id="codeline-14"><span class="cm">   You should have received a copy of the GNU Lesser General Public</span>
</span><span id="codeline-15"><span class="cm">   License along with the GNU C Library; if not, see</span>
</span><span id="codeline-16"><span class="cm">   &lt;https://www.gnu.org/licenses/&gt;.  */</span>
</span><span id="codeline-17">
</span><span id="codeline-18"><span class="cm">/*</span>
</span><span id="codeline-19"><span class="cm">   Copyright (C) 1983 Regents of the University of California.</span>
</span><span id="codeline-20"><span class="cm">   All rights reserved.</span>
</span><span id="codeline-21">
</span><span id="codeline-22"><span class="cm">   Redistribution and use in source and binary forms, with or without</span>
</span><span id="codeline-23"><span class="cm">   modification, are permitted provided that the following conditions</span>
</span><span id="codeline-24"><span class="cm">   are met:</span>
</span><span id="codeline-25">
</span><span id="codeline-26"><span class="cm">   1. Redistributions of source code must retain the above copyright</span>
</span><span id="codeline-27"><span class="cm">      notice, this list of conditions and the following disclaimer.</span>
</span><span id="codeline-28"><span class="cm">   2. Redistributions in binary form must reproduce the above copyright</span>
</span><span id="codeline-29"><span class="cm">      notice, this list of conditions and the following disclaimer in the</span>
</span><span id="codeline-30"><span class="cm">      documentation and/or other materials provided with the distribution.</span>
</span><span id="codeline-31"><span class="cm">   4. Neither the name of the University nor the names of its contributors</span>
</span><span id="codeline-32"><span class="cm">      may be used to endorse or promote products derived from this software</span>
</span><span id="codeline-33"><span class="cm">      without specific prior written permission.</span>
</span><span id="codeline-34">
</span><span id="codeline-35"><span class="cm">   THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS&#39;&#39; AND</span>
</span><span id="codeline-36"><span class="cm">   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE</span>
</span><span id="codeline-37"><span class="cm">   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE</span>
</span><span id="codeline-38"><span class="cm">   ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE</span>
</span><span id="codeline-39"><span class="cm">   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL</span>
</span><span id="codeline-40"><span class="cm">   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS</span>
</span><span id="codeline-41"><span class="cm">   OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)</span>
</span><span id="codeline-42"><span class="cm">   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT</span>
</span><span id="codeline-43"><span class="cm">   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY</span>
</span><span id="codeline-44"><span class="cm">   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF</span>
</span><span id="codeline-45"><span class="cm">   SUCH DAMAGE.*/</span>
</span><span id="codeline-46">
</span><span id="codeline-47"><span class="cm">/*</span>
</span><span id="codeline-48"><span class="cm"> * This is derived from the Berkeley source:</span>
</span><span id="codeline-49"><span class="cm"> *	@(#)random.c	5.5 (Berkeley) 7/6/88</span>
</span><span id="codeline-50"><span class="cm"> * It was reworked for the GNU C Library by Roland McGrath.</span>
</span><span id="codeline-51"><span class="cm"> * Rewritten to be reentrant by Ulrich Drepper, 1995</span>
</span><span id="codeline-52"><span class="cm"> */</span>
</span><span id="codeline-53">
</span><span id="codeline-54"><span class="cp">#include</span><span class="w"> </span><span class="cpf">&lt;errno.h&gt;</span>
</span><span id="codeline-55"><span class="cp">#include</span><span class="w"> </span><span class="cpf">&lt;limits.h&gt;</span>
</span><span id="codeline-56"><span class="cp">#include</span><span class="w"> </span><span class="cpf">&lt;stddef.h&gt;</span>
</span><span id="codeline-57"><span class="cp">#include</span><span class="w"> </span><span class="cpf">&lt;stdlib.h&gt;</span>
</span><span id="codeline-58"><span class="cp">#include</span><span class="w"> </span><span class="cpf">&lt;string.h&gt;</span>
</span><span id="codeline-59">
</span><span id="codeline-60">
</span><span id="codeline-61"><span class="cm">/* An improved random number generation package.  In addition to the standard</span>
</span><span id="codeline-62"><span class="cm">   rand()/srand() like interface, this package also has a special state info</span>
</span><span id="codeline-63"><span class="cm">   interface.  The initstate() routine is called with a seed, an array of</span>
</span><span id="codeline-64"><span class="cm">   bytes, and a count of how many bytes are being passed in; this array is</span>
</span><span id="codeline-65"><span class="cm">   then initialized to contain information for random number generation with</span>
</span><span id="codeline-66"><span class="cm">   that much state information.  Good sizes for the amount of state</span>
</span><span id="codeline-67"><span class="cm">   information are 32, 64, 128, and 256 bytes.  The state can be switched by</span>
</span><span id="codeline-68"><span class="cm">   calling the setstate() function with the same array as was initialized</span>
</span><span id="codeline-69"><span class="cm">   with initstate().  By default, the package runs with 128 bytes of state</span>
</span><span id="codeline-70"><span class="cm">   information and generates far better random numbers than a linear</span>
</span><span id="codeline-71"><span class="cm">   congruential generator.  If the amount of state information is less than</span>
</span><span id="codeline-72"><span class="cm">   32 bytes, a simple linear congruential R.N.G. is used.  Internally, the</span>
</span><span id="codeline-73"><span class="cm">   state information is treated as an array of longs; the zeroth element of</span>
</span><span id="codeline-74"><span class="cm">   the array is the type of R.N.G. being used (small integer); the remainder</span>
</span><span id="codeline-75"><span class="cm">   of the array is the state information for the R.N.G.  Thus, 32 bytes of</span>
</span><span id="codeline-76"><span class="cm">   state information will give 7 longs worth of state information, which will</span>
</span><span id="codeline-77"><span class="cm">   allow a degree seven polynomial.  (Note: The zeroth word of state</span>
</span><span id="codeline-78"><span class="cm">   information also has some other information stored in it; see setstate</span>
</span><span id="codeline-79"><span class="cm">   for details).  The random number generation technique is a linear feedback</span>
</span><span id="codeline-80"><span class="cm">   shift register approach, employing trinomials (since there are fewer terms</span>
</span><span id="codeline-81"><span class="cm">   to sum up that way).  In this approach, the least significant bit of all</span>
</span><span id="codeline-82"><span class="cm">   the numbers in the state table will act as a linear feedback shift register,</span>
</span><span id="codeline-83"><span class="cm">   and will have period 2^deg - 1 (where deg is the degree of the polynomial</span>
</span><span id="codeline-84"><span class="cm">   being used, assuming that the polynomial is irreducible and primitive).</span>
</span><span id="codeline-85"><span class="cm">   The higher order bits will have longer periods, since their values are</span>
</span><span id="codeline-86"><span class="cm">   also influenced by pseudo-random carries out of the lower bits.  The</span>
</span><span id="codeline-87"><span class="cm">   total period of the generator is approximately deg*(2**deg - 1); thus</span>
</span><span id="codeline-88"><span class="cm">   doubling the amount of state information has a vast influence on the</span>
</span><span id="codeline-89"><span class="cm">   period of the generator.  Note: The deg*(2**deg - 1) is an approximation</span>
</span><span id="codeline-90"><span class="cm">   only good for large deg, when the period of the shift register is the</span>
</span><span id="codeline-91"><span class="cm">   dominant factor.  With deg equal to seven, the period is actually much</span>
</span><span id="codeline-92"><span class="cm">   longer than the 7*(2**7 - 1) predicted by this formula.  */</span>
</span><span id="codeline-93">
</span><span id="codeline-94">
</span><span id="codeline-95">
</span><span id="codeline-96"><span class="cm">/* For each of the currently supported random number generators, we have a</span>
</span><span id="codeline-97"><span class="cm">   break value on the amount of state information (you need at least this many</span>
</span><span id="codeline-98"><span class="cm">   bytes of state info to support this random number generator), a degree for</span>
</span><span id="codeline-99"><span class="cm">   the polynomial (actually a trinomial) that the R.N.G. is based on, and</span>
</span><span id="codeline-100"><span class="cm">   separation between the two lower order coefficients of the trinomial.  */</span>
</span><span id="codeline-101">
</span><span id="codeline-102"><span class="cm">/* Linear congruential.  */</span>
</span><span id="codeline-103"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_0">TYPE_0</a>		0</span>
</span><span id="codeline-104"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/BREAK_0">BREAK_0</a>		8</span>
</span><span id="codeline-105"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/DEG_0">DEG_0</a>		0</span>
</span><span id="codeline-106"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/SEP_0">SEP_0</a>		0</span>
</span><span id="codeline-107">
</span><span id="codeline-108"><span class="cm">/* x**7 + x**3 + 1.  */</span>
</span><span id="codeline-109"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_1">TYPE_1</a>		1</span>
</span><span id="codeline-110"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/BREAK_1">BREAK_1</a>		32</span>
</span><span id="codeline-111"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/DEG_1">DEG_1</a>		7</span>
</span><span id="codeline-112"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/SEP_1">SEP_1</a>		3</span>
</span><span id="codeline-113">
</span><span id="codeline-114"><span class="cm">/* x**15 + x + 1.  */</span>
</span><span id="codeline-115"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_2">TYPE_2</a>		2</span>
</span><span id="codeline-116"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/BREAK_2">BREAK_2</a>		64</span>
</span><span id="codeline-117"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/DEG_2">DEG_2</a>		15</span>
</span><span id="codeline-118"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/SEP_2">SEP_2</a>		1</span>
</span><span id="codeline-119">
</span><span id="codeline-120"><span class="cm">/* x**31 + x**3 + 1.  */</span>
</span><span id="codeline-121"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_3">TYPE_3</a>		3</span>
</span><span id="codeline-122"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/BREAK_3">BREAK_3</a>		128</span>
</span><span id="codeline-123"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/DEG_3">DEG_3</a>		31</span>
</span><span id="codeline-124"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/SEP_3">SEP_3</a>		3</span>
</span><span id="codeline-125">
</span><span id="codeline-126"><span class="cm">/* x**63 + x + 1.  */</span>
</span><span id="codeline-127"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_4">TYPE_4</a>		4</span>
</span><span id="codeline-128"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/BREAK_4">BREAK_4</a>		256</span>
</span><span id="codeline-129"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/DEG_4">DEG_4</a>		63</span>
</span><span id="codeline-130"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/SEP_4">SEP_4</a>		1</span>
</span><span id="codeline-131">
</span><span id="codeline-132">
</span><span id="codeline-133"><span class="cm">/* Array versions of the above information to make code run faster.</span>
</span><span id="codeline-134"><span class="cm">   Relies on fact that TYPE_i == i.  */</span>
</span><span id="codeline-135">
</span><span id="codeline-136"><span class="cp">#define	<a class="ident" href="/glibc/glibc-2.42.9000/C/ident/MAX_TYPES">MAX_TYPES</a>	5	</span><span class="cm">/* Max number of types above.  */</span>
</span><span id="codeline-137">
</span><span id="codeline-138"><span class="k">struct</span><span class="w"> </span><span class="nc"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/random_poly_info">random_poly_info</a></span>
</span><span id="codeline-139"><span class="p">{</span>
</span><span id="codeline-140"><span class="w">  </span><span class="kt">int</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/seps">seps</a></span><span class="p">[</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/MAX_TYPES">MAX_TYPES</a></span><span class="p">];</span>
</span><span id="codeline-141"><span class="w">  </span><span class="kt">int</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/degrees">degrees</a></span><span class="p">[</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/MAX_TYPES">MAX_TYPES</a></span><span class="p">];</span>
</span><span id="codeline-142"><span class="p">};</span>
</span><span id="codeline-143">
</span><span id="codeline-144"><span class="k">static</span><span class="w"> </span><span class="k">const</span><span class="w"> </span><span class="k">struct</span><span class="w"> </span><span class="nc"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/random_poly_info">random_poly_info</a></span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/random_poly_info">random_poly_info</a></span><span class="w"> </span><span class="o">=</span>
</span><span id="codeline-145"><span class="p">{</span>
</span><span id="codeline-146"><span class="w">  </span><span class="p">{</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/SEP_0">SEP_0</a></span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/SEP_1">SEP_1</a></span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/SEP_2">SEP_2</a></span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/SEP_3">SEP_3</a></span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/SEP_4">SEP_4</a></span><span class="w"> </span><span class="p">},</span>
</span><span id="codeline-147"><span class="w">  </span><span class="p">{</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/DEG_0">DEG_0</a></span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/DEG_1">DEG_1</a></span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/DEG_2">DEG_2</a></span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/DEG_3">DEG_3</a></span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/DEG_4">DEG_4</a></span><span class="w"> </span><span class="p">}</span>
</span><span id="codeline-148"><span class="p">};</span>
</span><span id="codeline-149">
</span><span id="codeline-150"><span class="k">static</span><span class="w"> </span><span class="kr">inline</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span>
</span><span id="codeline-151"><span class="nf"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/read_state">read_state</a></span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="o">*</span><span class="n">b</span><span class="p">,</span><span class="w"> </span><span class="kt">int</span><span class="w"> </span><span class="n">idx</span><span class="p">)</span>
</span><span id="codeline-152"><span class="p">{</span>
</span><span id="codeline-153"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="n">r</span><span class="p">;</span>
</span><span id="codeline-154"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/memcpy">memcpy</a></span><span class="w"> </span><span class="p">(</span><span class="o">&amp;</span><span class="n">r</span><span class="p">,</span><span class="w"> </span><span class="p">(</span><span class="kt">char</span><span class="w"> </span><span class="o">*</span><span class="p">)</span><span class="w"> </span><span class="n">b</span><span class="w"> </span><span class="o">+</span><span class="w"> </span><span class="n">idx</span><span class="w"> </span><span class="o">*</span><span class="w"> </span><span class="mi">4</span><span class="p">,</span><span class="w"> </span><span class="k">sizeof</span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="p">));</span>
</span><span id="codeline-155"><span class="w">  </span><span class="k">return</span><span class="w"> </span><span class="n">r</span><span class="p">;</span>
</span><span id="codeline-156"><span class="p">}</span>
</span><span id="codeline-157">
</span><span id="codeline-158"><span class="k">static</span><span class="w"> </span><span class="kr">inline</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/void">void</a></span>
</span><span id="codeline-159"><span class="nf"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/write_state">write_state</a></span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="o">*</span><span class="n">b</span><span class="p">,</span><span class="w"> </span><span class="kt">int</span><span class="w"> </span><span class="n">idx</span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="n">v</span><span class="p">)</span>
</span><span id="codeline-160"><span class="p">{</span>
</span><span id="codeline-161"><span class="w">  </span><span class="cm">/* Use literal 4 to avoid conversion to an unsigned type and pointer</span>
</span><span id="codeline-162"><span class="cm">     wraparound.  */</span>
</span><span id="codeline-163"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/memcpy">memcpy</a></span><span class="w"> </span><span class="p">((</span><span class="kt">char</span><span class="w"> </span><span class="o">*</span><span class="p">)</span><span class="w"> </span><span class="n">b</span><span class="w"> </span><span class="o">+</span><span class="w"> </span><span class="n">idx</span><span class="w"> </span><span class="o">*</span><span class="w"> </span><span class="mi">4</span><span class="p">,</span><span class="w"> </span><span class="o">&amp;</span><span class="n">v</span><span class="p">,</span><span class="w"> </span><span class="mi">4</span><span class="p">);</span>
</span><span id="codeline-164"><span class="p">}</span>
</span><span id="codeline-165">
</span><span id="codeline-166"><span class="w"></span>
</span><span id="codeline-167"><span class="cm">/* Initialize the random number generator based on the given seed.  If the</span>
</span><span id="codeline-168"><span class="cm">   type is the trivial no-state-information type, just remember the seed.</span>
</span><span id="codeline-169"><span class="cm">   Otherwise, initializes state[] based on the given &quot;seed&quot; via a linear</span>
</span><span id="codeline-170"><span class="cm">   congruential generator.  Then, the pointers are set to known locations</span>
</span><span id="codeline-171"><span class="cm">   that are exactly rand_sep places apart.  Lastly, it cycles the state</span>
</span><span id="codeline-172"><span class="cm">   information a given number of times to get rid of any initial dependencies</span>
</span><span id="codeline-173"><span class="cm">   introduced by the L.C.R.N.G.  Note that the initialization of randtbl[]</span>
</span><span id="codeline-174"><span class="cm">   for default usage relies on values produced by this routine.  */</span>
</span><span id="codeline-175"><span class="kt">int</span>
</span><span id="codeline-176"><span class="nf"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/__srandom_r">__srandom_r</a></span><span class="w"> </span><span class="p">(</span><span class="kt">unsigned</span><span class="w"> </span><span class="kt">int</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/seed">seed</a></span><span class="p">,</span><span class="w"> </span><span class="k">struct</span><span class="w"> </span><span class="nc"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/random_data">random_data</a></span><span class="w"> </span><span class="o">*</span><span class="n">buf</span><span class="p">)</span>
</span><span id="codeline-177"><span class="p">{</span>
</span><span id="codeline-178"><span class="w">  </span><span class="kt">int</span><span class="w"> </span><span class="n">type</span><span class="p">;</span>
</span><span id="codeline-179"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="o">*</span><span class="n">state</span><span class="p">;</span>
</span><span id="codeline-180"><span class="w">  </span><span class="kt">long</span><span class="w"> </span><span class="kt">int</span><span class="w"> </span><span class="n">i</span><span class="p">;</span>
</span><span id="codeline-181"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/word">word</a></span><span class="p">;</span>
</span><span id="codeline-182"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="o">*</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/dst">dst</a></span><span class="p">;</span>
</span><span id="codeline-183"><span class="w">  </span><span class="kt">int</span><span class="w"> </span><span class="n">kc</span><span class="p">;</span>
</span><span id="codeline-184">
</span><span id="codeline-185"><span class="w">  </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n">buf</span><span class="w"> </span><span class="o">==</span><span class="w"> </span><span class="nb">NULL</span><span class="p">)</span>
</span><span id="codeline-186"><span class="w">    </span><span class="k">goto</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fail">fail</a></span><span class="p">;</span>
</span><span id="codeline-187"><span class="w">  </span><span class="n">type</span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rand_type">rand_type</a></span><span class="p">;</span>
</span><span id="codeline-188"><span class="w">  </span><span class="k">if</span><span class="w"> </span><span class="p">((</span><span class="kt">unsigned</span><span class="w"> </span><span class="kt">int</span><span class="p">)</span><span class="w"> </span><span class="n">type</span><span class="w"> </span><span class="o">&gt;=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/MAX_TYPES">MAX_TYPES</a></span><span class="p">)</span>
</span><span id="codeline-189"><span class="w">    </span><span class="k">goto</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fail">fail</a></span><span class="p">;</span>
</span><span id="codeline-190">
</span><span id="codeline-191"><span class="w">  </span><span class="n">state</span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n">state</span><span class="p">;</span>
</span><span id="codeline-192"><span class="w">  </span><span class="cm">/* We must make sure the seed is not 0.  Take arbitrarily 1 in this case.  */</span>
</span><span id="codeline-193"><span class="w">  </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/seed">seed</a></span><span class="w"> </span><span class="o">==</span><span class="w"> </span><span class="mi">0</span><span class="p">)</span>
</span><span id="codeline-194"><span class="w">    </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/seed">seed</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="mi">1</span><span class="p">;</span>
</span><span id="codeline-195"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/write_state">write_state</a></span><span class="w"> </span><span class="p">(</span><span class="n">state</span><span class="p">,</span><span class="w"> </span><span class="mi">0</span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/seed">seed</a></span><span class="p">);</span>
</span><span id="codeline-196"><span class="w">  </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n">type</span><span class="w"> </span><span class="o">==</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_0">TYPE_0</a></span><span class="p">)</span>
</span><span id="codeline-197"><span class="w">    </span><span class="k">goto</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/done">done</a></span><span class="p">;</span>
</span><span id="codeline-198">
</span><span id="codeline-199"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/dst">dst</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">state</span><span class="p">;</span>
</span><span id="codeline-200"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/word">word</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/seed">seed</a></span><span class="p">;</span>
</span><span id="codeline-201"><span class="w">  </span><span class="n">kc</span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rand_deg">rand_deg</a></span><span class="p">;</span>
</span><span id="codeline-202"><span class="w">  </span><span class="k">for</span><span class="w"> </span><span class="p">(</span><span class="n">i</span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="mi">1</span><span class="p">;</span><span class="w"> </span><span class="n">i</span><span class="w"> </span><span class="o">&lt;</span><span class="w"> </span><span class="n">kc</span><span class="p">;</span><span class="w"> </span><span class="o">++</span><span class="n">i</span><span class="p">)</span>
</span><span id="codeline-203"><span class="w">    </span><span class="p">{</span>
</span><span id="codeline-204"><span class="w">      </span><span class="cm">/* This does:</span>
</span><span id="codeline-205"><span class="cm">	   state[i] = (16807 * state[i - 1]) % 2147483647;</span>
</span><span id="codeline-206"><span class="cm">	 but avoids overflowing 31 bits.  */</span>
</span><span id="codeline-207"><span class="w">      </span><span class="kt">long</span><span class="w"> </span><span class="kt">int</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/hi">hi</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/word">word</a></span><span class="w"> </span><span class="o">/</span><span class="w"> </span><span class="mi">127773</span><span class="p">;</span>
</span><span id="codeline-208"><span class="w">      </span><span class="kt">long</span><span class="w"> </span><span class="kt">int</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/lo">lo</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/word">word</a></span><span class="w"> </span><span class="o">%</span><span class="w"> </span><span class="mi">127773</span><span class="p">;</span>
</span><span id="codeline-209"><span class="w">      </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/word">word</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="mi">16807</span><span class="w"> </span><span class="o">*</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/lo">lo</a></span><span class="w"> </span><span class="o">-</span><span class="w"> </span><span class="mi">2836</span><span class="w"> </span><span class="o">*</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/hi">hi</a></span><span class="p">;</span>
</span><span id="codeline-210"><span class="w">      </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/word">word</a></span><span class="w"> </span><span class="o">&lt;</span><span class="w"> </span><span class="mi">0</span><span class="p">)</span>
</span><span id="codeline-211"><span class="w">	</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/word">word</a></span><span class="w"> </span><span class="o">+=</span><span class="w"> </span><span class="mi">2147483647</span><span class="p">;</span>
</span><span id="codeline-212"><span class="w">      </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/write_state">write_state</a></span><span class="w"> </span><span class="p">(</span><span class="o">++</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/dst">dst</a></span><span class="p">,</span><span class="w"> </span><span class="mi">0</span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/word">word</a></span><span class="p">);</span>
</span><span id="codeline-213"><span class="w">    </span><span class="p">}</span>
</span><span id="codeline-214">
</span><span id="codeline-215"><span class="w">  </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fptr">fptr</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="o">&amp;</span><span class="n">state</span><span class="p">[</span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rand_sep">rand_sep</a></span><span class="p">];</span>
</span><span id="codeline-216"><span class="w">  </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rptr">rptr</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="o">&amp;</span><span class="n">state</span><span class="p">[</span><span class="mi">0</span><span class="p">];</span>
</span><span id="codeline-217"><span class="w">  </span><span class="n">kc</span><span class="w"> </span><span class="o">*=</span><span class="w"> </span><span class="mi">10</span><span class="p">;</span>
</span><span id="codeline-218"><span class="w">  </span><span class="k">while</span><span class="w"> </span><span class="p">(</span><span class="o">--</span><span class="n">kc</span><span class="w"> </span><span class="o">&gt;=</span><span class="w"> </span><span class="mi">0</span><span class="p">)</span>
</span><span id="codeline-219"><span class="w">    </span><span class="p">{</span>
</span><span id="codeline-220"><span class="w">      </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/discard">discard</a></span><span class="p">;</span>
</span><span id="codeline-221"><span class="w">      </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/void">void</a></span><span class="p">)</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/__random_r">__random_r</a></span><span class="w"> </span><span class="p">(</span><span class="n">buf</span><span class="p">,</span><span class="w"> </span><span class="o">&amp;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/discard">discard</a></span><span class="p">);</span>
</span><span id="codeline-222"><span class="w">    </span><span class="p">}</span>
</span><span id="codeline-223">
</span><span id="codeline-224"><span class="w"> </span><span class="nl"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/done">done</a></span><span class="p">:</span>
</span><span id="codeline-225"><span class="w">  </span><span class="k">return</span><span class="w"> </span><span class="mi">0</span><span class="p">;</span>
</span><span id="codeline-226">
</span><span id="codeline-227"><span class="w"> </span><span class="nl"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fail">fail</a></span><span class="p">:</span>
</span><span id="codeline-228"><span class="w">  </span><span class="k">return</span><span class="w"> </span><span class="mi">-1</span><span class="p">;</span>
</span><span id="codeline-229"><span class="p">}</span>
</span><span id="codeline-230">
</span><span id="codeline-231"><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/weak_alias">weak_alias</a></span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/__srandom_r">__srandom_r</a></span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/srandom_r">srandom_r</a></span><span class="p">)</span>
</span><span id="codeline-232"><span class="w"></span>
</span><span id="codeline-233"><span class="cm">/* Initialize the state information in the given array of N bytes for</span>
</span><span id="codeline-234"><span class="cm">   future random number generation.  Based on the number of bytes we</span>
</span><span id="codeline-235"><span class="cm">   are given, and the break values for the different R.N.G.&#39;s, we choose</span>
</span><span id="codeline-236"><span class="cm">   the best (largest) one we can and set things up for it.  srandom is</span>
</span><span id="codeline-237"><span class="cm">   then called to initialize the state information.  Note that on return</span>
</span><span id="codeline-238"><span class="cm">   from srandom, we set state[-1] to be the type multiplexed with the current</span>
</span><span id="codeline-239"><span class="cm">   value of the rear pointer; this is so successive calls to initstate won&#39;t</span>
</span><span id="codeline-240"><span class="cm">   lose this information and will be able to restart with setstate.</span>
</span><span id="codeline-241"><span class="cm">   Note: The first thing we do is save the current state, if any, just like</span>
</span><span id="codeline-242"><span class="cm">   setstate so that it doesn&#39;t matter when initstate is called.</span>
</span><span id="codeline-243"><span class="cm">   Returns 0 on success, non-zero on failure.  */</span>
</span><span id="codeline-244"><span class="kt">int</span>
</span><span id="codeline-245"><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/__initstate_r">__initstate_r</a></span><span class="w"> </span><span class="p">(</span><span class="kt">unsigned</span><span class="w"> </span><span class="kt">int</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/seed">seed</a></span><span class="p">,</span><span class="w"> </span><span class="kt">char</span><span class="w"> </span><span class="o">*</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/arg_state">arg_state</a></span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/size_t">size_t</a></span><span class="w"> </span><span class="n">n</span><span class="p">,</span>
</span><span id="codeline-246"><span class="w">	       </span><span class="k">struct</span><span class="w"> </span><span class="nc"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/random_data">random_data</a></span><span class="w"> </span><span class="o">*</span><span class="n">buf</span><span class="p">)</span>
</span><span id="codeline-247"><span class="p">{</span>
</span><span id="codeline-248"><span class="w">  </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n">buf</span><span class="w"> </span><span class="o">==</span><span class="w"> </span><span class="nb">NULL</span><span class="p">)</span>
</span><span id="codeline-249"><span class="w">    </span><span class="k">goto</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fail">fail</a></span><span class="p">;</span>
</span><span id="codeline-250">
</span><span id="codeline-251"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="o">*</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/old_state">old_state</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n">state</span><span class="p">;</span>
</span><span id="codeline-252"><span class="w">  </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/old_state">old_state</a></span><span class="w"> </span><span class="o">!=</span><span class="w"> </span><span class="nb">NULL</span><span class="p">)</span>
</span><span id="codeline-253"><span class="w">    </span><span class="p">{</span>
</span><span id="codeline-254"><span class="w">      </span><span class="kt">int</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/old_type">old_type</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rand_type">rand_type</a></span><span class="p">;</span>
</span><span id="codeline-255"><span class="w">      </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/old_type">old_type</a></span><span class="w"> </span><span class="o">==</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_0">TYPE_0</a></span><span class="p">)</span>
</span><span id="codeline-256"><span class="w">	</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/write_state">write_state</a></span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/old_state">old_state</a></span><span class="p">,</span><span class="w"> </span><span class="mi">-1</span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_0">TYPE_0</a></span><span class="p">);</span>
</span><span id="codeline-257"><span class="w">      </span><span class="k">else</span>
</span><span id="codeline-258"><span class="w">	</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/write_state">write_state</a></span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/old_state">old_state</a></span><span class="p">,</span><span class="w"> </span><span class="mi">-1</span><span class="p">,</span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/MAX_TYPES">MAX_TYPES</a></span><span class="w"> </span><span class="o">*</span><span class="w"> </span><span class="p">(</span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rptr">rptr</a></span><span class="w"> </span><span class="o">-</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/old_state">old_state</a></span><span class="p">))</span>
</span><span id="codeline-259"><span class="w">				    </span><span class="o">+</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/old_type">old_type</a></span><span class="p">);</span>
</span><span id="codeline-260"><span class="w">    </span><span class="p">}</span>
</span><span id="codeline-261">
</span><span id="codeline-262"><span class="w">  </span><span class="kt">int</span><span class="w"> </span><span class="n">type</span><span class="p">;</span>
</span><span id="codeline-263"><span class="w">  </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n">n</span><span class="w"> </span><span class="o">&gt;=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/BREAK_3">BREAK_3</a></span><span class="p">)</span>
</span><span id="codeline-264"><span class="w">    </span><span class="n">type</span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">n</span><span class="w"> </span><span class="o">&lt;</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/BREAK_4">BREAK_4</a></span><span class="w"> </span><span class="o">?</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_3">TYPE_3</a></span><span class="w"> </span><span class="o">:</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_4">TYPE_4</a></span><span class="p">;</span>
</span><span id="codeline-265"><span class="w">  </span><span class="k">else</span><span class="w"> </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n">n</span><span class="w"> </span><span class="o">&lt;</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/BREAK_1">BREAK_1</a></span><span class="p">)</span>
</span><span id="codeline-266"><span class="w">    </span><span class="p">{</span>
</span><span id="codeline-267"><span class="w">      </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n">n</span><span class="w"> </span><span class="o">&lt;</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/BREAK_0">BREAK_0</a></span><span class="p">)</span>
</span><span id="codeline-268"><span class="w">	</span><span class="k">goto</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fail">fail</a></span><span class="p">;</span>
</span><span id="codeline-269">
</span><span id="codeline-270"><span class="w">      </span><span class="n">type</span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_0">TYPE_0</a></span><span class="p">;</span>
</span><span id="codeline-271"><span class="w">    </span><span class="p">}</span>
</span><span id="codeline-272"><span class="w">  </span><span class="k">else</span>
</span><span id="codeline-273"><span class="w">    </span><span class="n">type</span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">n</span><span class="w"> </span><span class="o">&lt;</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/BREAK_2">BREAK_2</a></span><span class="w"> </span><span class="o">?</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_1">TYPE_1</a></span><span class="w"> </span><span class="o">:</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_2">TYPE_2</a></span><span class="p">;</span>
</span><span id="codeline-274">
</span><span id="codeline-275"><span class="w">  </span><span class="kt">int</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/degree">degree</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/random_poly_info">random_poly_info</a></span><span class="p">.</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/degrees">degrees</a></span><span class="p">[</span><span class="n">type</span><span class="p">];</span>
</span><span id="codeline-276"><span class="w">  </span><span class="kt">int</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/separation">separation</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/random_poly_info">random_poly_info</a></span><span class="p">.</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/seps">seps</a></span><span class="p">[</span><span class="n">type</span><span class="p">];</span>
</span><span id="codeline-277">
</span><span id="codeline-278"><span class="w">  </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rand_type">rand_type</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">type</span><span class="p">;</span>
</span><span id="codeline-279"><span class="w">  </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rand_sep">rand_sep</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/separation">separation</a></span><span class="p">;</span>
</span><span id="codeline-280"><span class="w">  </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rand_deg">rand_deg</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/degree">degree</a></span><span class="p">;</span>
</span><span id="codeline-281"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="o">*</span><span class="n">state</span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="o">&amp;</span><span class="p">((</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="o">*</span><span class="p">)</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/arg_state">arg_state</a></span><span class="p">)[</span><span class="mi">1</span><span class="p">];</span><span class="w">	</span><span class="cm">/* First location.  */</span>
</span><span id="codeline-282"><span class="w">  </span><span class="cm">/* Must set END_PTR before srandom.  */</span>
</span><span id="codeline-283"><span class="w">  </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/end_ptr">end_ptr</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="o">&amp;</span><span class="n">state</span><span class="p">[</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/degree">degree</a></span><span class="p">];</span>
</span><span id="codeline-284">
</span><span id="codeline-285"><span class="w">  </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n">state</span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">state</span><span class="p">;</span>
</span><span id="codeline-286">
</span><span id="codeline-287"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/__srandom_r">__srandom_r</a></span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/seed">seed</a></span><span class="p">,</span><span class="w"> </span><span class="n">buf</span><span class="p">);</span>
</span><span id="codeline-288">
</span><span id="codeline-289"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/write_state">write_state</a></span><span class="w"> </span><span class="p">(</span><span class="n">state</span><span class="p">,</span><span class="w"> </span><span class="mi">-1</span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_0">TYPE_0</a></span><span class="p">);</span>
</span><span id="codeline-290"><span class="w">  </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n">type</span><span class="w"> </span><span class="o">!=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_0">TYPE_0</a></span><span class="p">)</span>
</span><span id="codeline-291"><span class="w">    </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/write_state">write_state</a></span><span class="w"> </span><span class="p">(</span><span class="n">state</span><span class="p">,</span><span class="w"> </span><span class="mi">-1</span><span class="p">,</span><span class="w"> </span><span class="p">(</span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rptr">rptr</a></span><span class="w"> </span><span class="o">-</span><span class="w"> </span><span class="n">state</span><span class="p">)</span><span class="w"> </span><span class="o">*</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/MAX_TYPES">MAX_TYPES</a></span><span class="w"> </span><span class="o">+</span><span class="w"> </span><span class="n">type</span><span class="p">);</span>
</span><span id="codeline-292">
</span><span id="codeline-293"><span class="w">  </span><span class="k">return</span><span class="w"> </span><span class="mi">0</span><span class="p">;</span>
</span><span id="codeline-294">
</span><span id="codeline-295"><span class="w"> </span><span class="nl"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fail">fail</a></span><span class="p">:</span>
</span><span id="codeline-296"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/__set_errno">__set_errno</a></span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/EINVAL">EINVAL</a></span><span class="p">);</span>
</span><span id="codeline-297"><span class="w">  </span><span class="k">return</span><span class="w"> </span><span class="mi">-1</span><span class="p">;</span>
</span><span id="codeline-298"><span class="p">}</span>
</span><span id="codeline-299">
</span><span id="codeline-300"><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/weak_alias">weak_alias</a></span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/__initstate_r">__initstate_r</a></span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/initstate_r">initstate_r</a></span><span class="p">)</span>
</span><span id="codeline-301"><span class="w"></span>
</span><span id="codeline-302"><span class="cm">/* Restore the state from the given state array.</span>
</span><span id="codeline-303"><span class="cm">   Note: It is important that we also remember the locations of the pointers</span>
</span><span id="codeline-304"><span class="cm">   in the current state information, and restore the locations of the pointers</span>
</span><span id="codeline-305"><span class="cm">   from the old state information.  This is done by multiplexing the pointer</span>
</span><span id="codeline-306"><span class="cm">   location into the zeroth word of the state information. Note that due</span>
</span><span id="codeline-307"><span class="cm">   to the order in which things are done, it is OK to call setstate with the</span>
</span><span id="codeline-308"><span class="cm">   same state as the current state</span>
</span><span id="codeline-309"><span class="cm">   Returns 0 on success, non-zero on failure.  */</span>
</span><span id="codeline-310"><span class="kt">int</span>
</span><span id="codeline-311"><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/__setstate_r">__setstate_r</a></span><span class="w"> </span><span class="p">(</span><span class="kt">char</span><span class="w"> </span><span class="o">*</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/arg_state">arg_state</a></span><span class="p">,</span><span class="w"> </span><span class="k">struct</span><span class="w"> </span><span class="nc"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/random_data">random_data</a></span><span class="w"> </span><span class="o">*</span><span class="n">buf</span><span class="p">)</span>
</span><span id="codeline-312"><span class="p">{</span>
</span><span id="codeline-313"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="o">*</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/new_state">new_state</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="mi">1</span><span class="w"> </span><span class="o">+</span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="o">*</span><span class="p">)</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/arg_state">arg_state</a></span><span class="p">;</span>
</span><span id="codeline-314"><span class="w">  </span><span class="kt">int</span><span class="w"> </span><span class="n">type</span><span class="p">;</span>
</span><span id="codeline-315"><span class="w">  </span><span class="kt">int</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/old_type">old_type</a></span><span class="p">;</span>
</span><span id="codeline-316"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="o">*</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/old_state">old_state</a></span><span class="p">;</span>
</span><span id="codeline-317"><span class="w">  </span><span class="kt">int</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/degree">degree</a></span><span class="p">;</span>
</span><span id="codeline-318"><span class="w">  </span><span class="kt">int</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/separation">separation</a></span><span class="p">;</span>
</span><span id="codeline-319">
</span><span id="codeline-320"><span class="w">  </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/arg_state">arg_state</a></span><span class="w"> </span><span class="o">==</span><span class="w"> </span><span class="nb">NULL</span><span class="w"> </span><span class="o">||</span><span class="w"> </span><span class="n">buf</span><span class="w"> </span><span class="o">==</span><span class="w"> </span><span class="nb">NULL</span><span class="p">)</span>
</span><span id="codeline-321"><span class="w">    </span><span class="k">goto</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fail">fail</a></span><span class="p">;</span>
</span><span id="codeline-322">
</span><span id="codeline-323"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/old_type">old_type</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rand_type">rand_type</a></span><span class="p">;</span>
</span><span id="codeline-324"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/old_state">old_state</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n">state</span><span class="p">;</span>
</span><span id="codeline-325"><span class="w">  </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/old_type">old_type</a></span><span class="w"> </span><span class="o">==</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_0">TYPE_0</a></span><span class="p">)</span>
</span><span id="codeline-326"><span class="w">    </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/write_state">write_state</a></span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/old_state">old_state</a></span><span class="p">,</span><span class="w"> </span><span class="mi">-1</span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_0">TYPE_0</a></span><span class="p">);</span>
</span><span id="codeline-327"><span class="w">  </span><span class="k">else</span>
</span><span id="codeline-328"><span class="w">    </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/write_state">write_state</a></span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/old_state">old_state</a></span><span class="p">,</span><span class="w"> </span><span class="mi">-1</span><span class="p">,</span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/MAX_TYPES">MAX_TYPES</a></span><span class="w"> </span><span class="o">*</span><span class="w"> </span><span class="p">(</span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rptr">rptr</a></span><span class="w"> </span><span class="o">-</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/old_state">old_state</a></span><span class="p">))</span>
</span><span id="codeline-329"><span class="w">				</span><span class="o">+</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/old_type">old_type</a></span><span class="p">);</span>
</span><span id="codeline-330">
</span><span id="codeline-331"><span class="w">  </span><span class="n">type</span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/new_state">new_state</a></span><span class="p">[</span><span class="mi">-1</span><span class="p">]</span><span class="w"> </span><span class="o">%</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/MAX_TYPES">MAX_TYPES</a></span><span class="p">;</span>
</span><span id="codeline-332"><span class="w">  </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n">type</span><span class="w"> </span><span class="o">&lt;</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_0">TYPE_0</a></span><span class="w"> </span><span class="o">||</span><span class="w"> </span><span class="n">type</span><span class="w"> </span><span class="o">&gt;</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_4">TYPE_4</a></span><span class="p">)</span>
</span><span id="codeline-333"><span class="w">    </span><span class="k">goto</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fail">fail</a></span><span class="p">;</span>
</span><span id="codeline-334">
</span><span id="codeline-335"><span class="w">  </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rand_deg">rand_deg</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/degree">degree</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/random_poly_info">random_poly_info</a></span><span class="p">.</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/degrees">degrees</a></span><span class="p">[</span><span class="n">type</span><span class="p">];</span>
</span><span id="codeline-336"><span class="w">  </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rand_sep">rand_sep</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/separation">separation</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/random_poly_info">random_poly_info</a></span><span class="p">.</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/seps">seps</a></span><span class="p">[</span><span class="n">type</span><span class="p">];</span>
</span><span id="codeline-337"><span class="w">  </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rand_type">rand_type</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">type</span><span class="p">;</span>
</span><span id="codeline-338">
</span><span id="codeline-339"><span class="w">  </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n">type</span><span class="w"> </span><span class="o">!=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_0">TYPE_0</a></span><span class="p">)</span>
</span><span id="codeline-340"><span class="w">    </span><span class="p">{</span>
</span><span id="codeline-341"><span class="w">      </span><span class="kt">int</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rear">rear</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/new_state">new_state</a></span><span class="p">[</span><span class="mi">-1</span><span class="p">]</span><span class="w"> </span><span class="o">/</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/MAX_TYPES">MAX_TYPES</a></span><span class="p">;</span>
</span><span id="codeline-342"><span class="w">      </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rptr">rptr</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="o">&amp;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/new_state">new_state</a></span><span class="p">[</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rear">rear</a></span><span class="p">];</span>
</span><span id="codeline-343"><span class="w">      </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fptr">fptr</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="o">&amp;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/new_state">new_state</a></span><span class="p">[(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rear">rear</a></span><span class="w"> </span><span class="o">+</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/separation">separation</a></span><span class="p">)</span><span class="w"> </span><span class="o">%</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/degree">degree</a></span><span class="p">];</span>
</span><span id="codeline-344"><span class="w">    </span><span class="p">}</span>
</span><span id="codeline-345"><span class="w">  </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n">state</span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/new_state">new_state</a></span><span class="p">;</span>
</span><span id="codeline-346"><span class="w">  </span><span class="cm">/* Set end_ptr too.  */</span>
</span><span id="codeline-347"><span class="w">  </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/end_ptr">end_ptr</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="o">&amp;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/new_state">new_state</a></span><span class="p">[</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/degree">degree</a></span><span class="p">];</span>
</span><span id="codeline-348">
</span><span id="codeline-349"><span class="w">  </span><span class="k">return</span><span class="w"> </span><span class="mi">0</span><span class="p">;</span>
</span><span id="codeline-350">
</span><span id="codeline-351"><span class="w"> </span><span class="nl"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fail">fail</a></span><span class="p">:</span>
</span><span id="codeline-352"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/__set_errno">__set_errno</a></span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/EINVAL">EINVAL</a></span><span class="p">);</span>
</span><span id="codeline-353"><span class="w">  </span><span class="k">return</span><span class="w"> </span><span class="mi">-1</span><span class="p">;</span>
</span><span id="codeline-354"><span class="p">}</span>
</span><span id="codeline-355">
</span><span id="codeline-356"><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/weak_alias">weak_alias</a></span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/__setstate_r">__setstate_r</a></span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/setstate_r">setstate_r</a></span><span class="p">)</span>
</span><span id="codeline-357"><span class="w"></span>
</span><span id="codeline-358"><span class="cm">/* If we are using the trivial TYPE_0 R.N.G., just do the old linear</span>
</span><span id="codeline-359"><span class="cm">   congruential bit.  Otherwise, we do our fancy trinomial stuff, which is the</span>
</span><span id="codeline-360"><span class="cm">   same in all the other cases due to all the global variables that have been</span>
</span><span id="codeline-361"><span class="cm">   set up.  The basic operation is to add the number at the rear pointer into</span>
</span><span id="codeline-362"><span class="cm">   the one at the front pointer.  Then both pointers are advanced to the next</span>
</span><span id="codeline-363"><span class="cm">   location cyclically in the table.  The value returned is the sum generated,</span>
</span><span id="codeline-364"><span class="cm">   reduced to 31 bits by throwing away the &quot;least random&quot; low bit.</span>
</span><span id="codeline-365"><span class="cm">   Note: The code takes advantage of the fact that both the front and</span>
</span><span id="codeline-366"><span class="cm">   rear pointers can&#39;t wrap on the same call by not testing the rear</span>
</span><span id="codeline-367"><span class="cm">   pointer if the front one has wrapped.  Returns a 31-bit random number.  */</span>
</span><span id="codeline-368">
</span><span id="codeline-369"><span class="kt">int</span>
</span><span id="codeline-370"><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/__random_r">__random_r</a></span><span class="w"> </span><span class="p">(</span><span class="k">struct</span><span class="w"> </span><span class="nc"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/random_data">random_data</a></span><span class="w"> </span><span class="o">*</span><span class="n">buf</span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="o">*</span><span class="n">result</span><span class="p">)</span>
</span><span id="codeline-371"><span class="p">{</span>
</span><span id="codeline-372"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="o">*</span><span class="n">state</span><span class="p">;</span>
</span><span id="codeline-373">
</span><span id="codeline-374"><span class="w">  </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n">buf</span><span class="w"> </span><span class="o">==</span><span class="w"> </span><span class="nb">NULL</span><span class="w"> </span><span class="o">||</span><span class="w"> </span><span class="n">result</span><span class="w"> </span><span class="o">==</span><span class="w"> </span><span class="nb">NULL</span><span class="p">)</span>
</span><span id="codeline-375"><span class="w">    </span><span class="k">goto</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fail">fail</a></span><span class="p">;</span>
</span><span id="codeline-376">
</span><span id="codeline-377"><span class="w">  </span><span class="n">state</span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n">state</span><span class="p">;</span>
</span><span id="codeline-378">
</span><span id="codeline-379"><span class="w">  </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rand_type">rand_type</a></span><span class="w"> </span><span class="o">==</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/TYPE_0">TYPE_0</a></span><span class="p">)</span>
</span><span id="codeline-380"><span class="w">    </span><span class="p">{</span>
</span><span id="codeline-381"><span class="w">      </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="n">val</span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="p">((</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/read_state">read_state</a></span><span class="p">(</span><span class="n">state</span><span class="p">,</span><span class="w"> </span><span class="mi">0</span><span class="p">)</span><span class="w"> </span><span class="o">*</span><span class="w"> </span><span class="mi">1103515245U</span><span class="p">)</span><span class="w"> </span><span class="o">+</span><span class="w"> </span><span class="mi">12345U</span><span class="p">)</span>
</span><span id="codeline-382"><span class="w">		     </span><span class="o">&amp;</span><span class="w"> </span><span class="mh">0x7fffffff</span><span class="p">;</span>
</span><span id="codeline-383"><span class="w">      </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/write_state">write_state</a></span><span class="w"> </span><span class="p">(</span><span class="n">state</span><span class="p">,</span><span class="w"> </span><span class="mi">0</span><span class="p">,</span><span class="w"> </span><span class="n">val</span><span class="p">);</span>
</span><span id="codeline-384"><span class="w">      </span><span class="o">*</span><span class="n">result</span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">val</span><span class="p">;</span>
</span><span id="codeline-385"><span class="w">    </span><span class="p">}</span>
</span><span id="codeline-386"><span class="w">  </span><span class="k">else</span>
</span><span id="codeline-387"><span class="w">    </span><span class="p">{</span>
</span><span id="codeline-388"><span class="w">      </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="o">*</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fptr">fptr</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fptr">fptr</a></span><span class="p">;</span>
</span><span id="codeline-389"><span class="w">      </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="o">*</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rptr">rptr</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rptr">rptr</a></span><span class="p">;</span>
</span><span id="codeline-390"><span class="w">      </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/int32_t">int32_t</a></span><span class="w"> </span><span class="o">*</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/end_ptr">end_ptr</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/end_ptr">end_ptr</a></span><span class="p">;</span>
</span><span id="codeline-391"><span class="w">      </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/uint32_t">uint32_t</a></span><span class="w"> </span><span class="n">val</span><span class="p">;</span>
</span><span id="codeline-392">
</span><span id="codeline-393"><span class="w">      </span><span class="cm">/* Avoid integer overflow with uint32_t arihmetic.  */</span>
</span><span id="codeline-394"><span class="w">      </span><span class="n">val</span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/read_state">read_state</a></span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fptr">fptr</a></span><span class="p">,</span><span class="w"> </span><span class="mi">0</span><span class="p">);</span>
</span><span id="codeline-395"><span class="w">      </span><span class="n">val</span><span class="w"> </span><span class="o">+=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/read_state">read_state</a></span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rptr">rptr</a></span><span class="p">,</span><span class="w"> </span><span class="mi">0</span><span class="p">);</span>
</span><span id="codeline-396"><span class="w">      </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/write_state">write_state</a></span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fptr">fptr</a></span><span class="p">,</span><span class="w"> </span><span class="mi">0</span><span class="p">,</span><span class="w"> </span><span class="n">val</span><span class="p">);</span>
</span><span id="codeline-397"><span class="w">      </span><span class="cm">/* Chucking least random bit.  */</span>
</span><span id="codeline-398"><span class="w">      </span><span class="o">*</span><span class="n">result</span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">val</span><span class="w"> </span><span class="o">&gt;&gt;</span><span class="w"> </span><span class="mi">1</span><span class="p">;</span>
</span><span id="codeline-399"><span class="w">      </span><span class="o">++</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fptr">fptr</a></span><span class="p">;</span>
</span><span id="codeline-400"><span class="w">      </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fptr">fptr</a></span><span class="w"> </span><span class="o">&gt;=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/end_ptr">end_ptr</a></span><span class="p">)</span>
</span><span id="codeline-401"><span class="w">	</span><span class="p">{</span>
</span><span id="codeline-402"><span class="w">	  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fptr">fptr</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">state</span><span class="p">;</span>
</span><span id="codeline-403"><span class="w">	  </span><span class="o">++</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rptr">rptr</a></span><span class="p">;</span>
</span><span id="codeline-404"><span class="w">	</span><span class="p">}</span>
</span><span id="codeline-405"><span class="w">      </span><span class="k">else</span>
</span><span id="codeline-406"><span class="w">	</span><span class="p">{</span>
</span><span id="codeline-407"><span class="w">	  </span><span class="o">++</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rptr">rptr</a></span><span class="p">;</span>
</span><span id="codeline-408"><span class="w">	  </span><span class="k">if</span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rptr">rptr</a></span><span class="w"> </span><span class="o">&gt;=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/end_ptr">end_ptr</a></span><span class="p">)</span>
</span><span id="codeline-409"><span class="w">	    </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rptr">rptr</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n">state</span><span class="p">;</span>
</span><span id="codeline-410"><span class="w">	</span><span class="p">}</span>
</span><span id="codeline-411"><span class="w">      </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fptr">fptr</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fptr">fptr</a></span><span class="p">;</span>
</span><span id="codeline-412"><span class="w">      </span><span class="n">buf</span><span class="o">-&gt;</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rptr">rptr</a></span><span class="w"> </span><span class="o">=</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/rptr">rptr</a></span><span class="p">;</span>
</span><span id="codeline-413"><span class="w">    </span><span class="p">}</span>
</span><span id="codeline-414"><span class="w">  </span><span class="k">return</span><span class="w"> </span><span class="mi">0</span><span class="p">;</span>
</span><span id="codeline-415">
</span><span id="codeline-416"><span class="w"> </span><span class="nl"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/fail">fail</a></span><span class="p">:</span>
</span><span id="codeline-417"><span class="w">  </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/__set_errno">__set_errno</a></span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/EINVAL">EINVAL</a></span><span class="p">);</span>
</span><span id="codeline-418"><span class="w">  </span><span class="k">return</span><span class="w"> </span><span class="mi">-1</span><span class="p">;</span>
</span><span id="codeline-419"><span class="p">}</span>
</span><span id="codeline-420">
</span><span id="codeline-421"><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/weak_alias">weak_alias</a></span><span class="w"> </span><span class="p">(</span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/__random_r">__random_r</a></span><span class="p">,</span><span class="w"> </span><span class="n"><a class="ident" href="/glibc/glibc-2.42.9000/C/ident/random_r">random_r</a></span><span class="p">)</span>
</span></pre></div></td></tr></table></div>

</div>

                </div>
            </main>
            <footer class="footer">
                <span class="version">
                    <a href="/glibc/glibc-2.42.9000/source">glibc</a>
                    <em class="icon-tag">glibc-2.42.9000</em>
                </span>
                
<span id="file-download-link">
    <a href="?raw">Download file</a>
</span>

                <a title="Go to top of the page" class="go-top icon-up screenreader" href="#">
                    Top
                </a>
                <span class="poweredby">powered by
                    <a target="_blank" href="https://github.com/bootlin/elixir/tree/854db2e">
                        Elixir 854db2e
                    </a>
                </span>
            </footer>
        </div>
        <script src="/static/script.js?v=17"></script>
        <script src="/static/dynamic-references.js?v=4"></script>
        <script src="/static/autocomplete.js" project="glibc"></script>
    </body>
</html>