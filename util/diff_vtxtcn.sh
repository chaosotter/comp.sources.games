#!/bin/bash

echo 'gtext.inc'
diff gtext.inc original/world/gtext.inc && echo '  OK'

echo 'objdes.inc'
diff objdes.inc original/world/objdes.inc && echo '  OK'

echo 'qtext.inc'
diff qtext.inc original/world/qtext.inc && echo '  OK'

echo 'q1text.dat'
diff q1text.dat original/world/q1text.dat && echo '  OK'
