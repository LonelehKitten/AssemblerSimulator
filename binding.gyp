{
  "targets": [
    {
      "target_name": "ASMR",
      "sources": [ 
        "src/bin/ASMR.cpp",
        "src/bin/sources/InterfaceBus.cpp",
        "src/bin/sources/ServiceBus.cpp",
        "src/bin/sources/Utils.cpp",
        "src/bin/sources/analyzer/LexiconAutomatons.cpp",
        "src/bin/sources/analyzer/LexiconScanner.cpp",
        "src/bin/sources/analyzer/LexiconScannerStatus.cpp",
        "src/bin/sources/analyzer/RecognitionManager.cpp",
        "src/bin/sources/analyzer/Semantic.cpp",
        "src/bin/sources/analyzer/SyntaxAnalyzer.cpp",
        "src/bin/sources/analyzer/SyntaxAutomatons.cpp",
        "src/bin/sources/analyzer/Token.cpp",
        "src/bin/sources/assembler/Assembler.cpp",
        "src/bin/sources/assembler/MacroDef.cpp",
        "src/bin/sources/assembler/Symbol.cpp",
        "src/bin/sources/assembler/SegmentDef.cpp",
        "src/bin/sources/assembler/ExpressionEvaluator.cpp",
        "src/bin/sources/machine/Z808Processor.cpp",
        "src/bin/sources/machine/Z808Response.cpp",
        "src/bin/sources/machine/Z808Machine.cpp",
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}