{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ 
        "src/bin/addon.cc",
        "src/bin/sources/analyzer/LexiconAutomatons.cpp",
        "src/bin/sources/analyzer/LexiconScanner.cpp",
        "src/bin/sources/analyzer/LexiconScannerStatus.cpp",
        "src/bin/sources/analyzer/RecognitionManager.cpp",
        "src/bin/sources/analyzer/Semantic.cpp",
        "src/bin/sources/analyzer/SyntaxAnalyzer.cpp",
        "src/bin/sources/analyzer/SyntaxAutomatons.cpp",
        "src/bin/sources/analyzer/Token.cpp",
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}