    #!/bin/bash
        for i in `seq $1 $2`; do
            ./test_proj.sh testcases/testmd-$i > testcases/expected/testmd-$i-expected
        done