rm $(dirname $0)/test.html
eval $EDITOR $(dirname $0)/test.html

GENERIC_TEST="$(cat $(dirname $0)/test.html)"
LTR_TEST="$(cat $(dirname $0)/test.html)"
RTL_TEST="$(cat $(dirname $0)/test.html)"

# Instead of using vendored prefixes such as "-webkit-flex-end", we take an
# approach that allows us to easily test in multiple browser engines, by
# ahead-of-time compiling start/end to the respective left/right.
#
# This will cause markup such as:
#
#   <div style=
#     "margin-replaceWithActualStart-because-start: 1;
#      margin-replaceWithActualStart:0px;
#
#      margin-replaceWithActualEnd-because-end: 1;
#      margin-replaceWithActualEnd:1px;
#
#      replaceWithActualStart: 0;
#
#      replaceWithActualEnd-because-end: 1;
#      replaceWithActualEnd: 2px"
#   />
#
# To become:
#
#   <div style=
#     "margin-left-because-start: 1;
#      margin-left:0px;
#
#      margin-end-because-end: 1;
#      margin-end:1px;
#
#      left: 0;
#
#      end-because-end: 1;
#      end: 2px"
#   />

# Which allows us to easily see *why* a property such as margin-left was set.
# Was it because we used margin-start, margin-end, or margin-left?
#
LTR_TEST=${LTR_TEST//replaceWithActualStart/left}
LTR_TEST=${LTR_TEST//replaceWithActualEnd/right}

RTL_TEST=${RTL_TEST//replaceWithActualStart/right}
RTL_TEST=${RTL_TEST//replaceWithActualEnd/left}

printf "$(cat $(dirname $0)/test-template.html)" "$LTR_TEST" "$RTL_TEST" > $(dirname $0)/test.html
open $(dirname $0)/test.html

