set_project("NayukiYq")

set_xmakever("2.9.2")

set_languages("cxx20")


-- add engine
includes("NayukiYq")

-- add tests
includes("Tests")

-- add examples
includes("Examples/Window")
includes("Examples/BoxApp")