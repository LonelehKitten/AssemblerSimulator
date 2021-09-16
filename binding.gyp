{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "src/bin/addon.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}