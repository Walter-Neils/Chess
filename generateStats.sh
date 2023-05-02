#!/bin/bash

# Delete stats.md if it exists
rm -f stats.md

# Create stats.md
touch stats.md

metrix++ collect --std.code.lines.code --std.code.lines.preprocessor --std.code.complexity.cyclomatic --std.code.complexity.maxindent --std.code.magic.numbers --std.code.maintindex.simple

cloc_output=$(cloc ./ --quiet --md | tail -n +3 | head -n -1)
metrixpp_output=$(metrix++ view)

echo "## Lines of Code" >>stats.md
echo "$cloc_output" >>stats.md

echo "## Metrix++" >>stats.md

# Prepare code block
echo '```' >>stats.md
echo "$metrixpp_output" >>stats.md
echo '```' >>stats.md

rm -f ./metrixpp.db
