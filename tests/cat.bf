use mem 0 = running
+         set running = 1
[         while running
    >         mem 1: input store
    ,         read stdin

    using if(x){}else{} algorithm by Ben-Arba on https://esolangs.org/wiki/Brainfuck_algorithms
    use x = mem 1, temp0 = mem 2, temp1 = mem 3
    >+<       define temp0 = 1, temp1 = 0
    [        if x
        .        write stdout
        >-]      set temp0 = 0
    >[        else we are at x. Add 1, we are at x ? temp1 (0) : temp0 (1)
        <<-       set running = 0 (originally 1)
        >>-       cleanup temp0 = 0
        >]        go to temp1. We must now be at temp1 (0).
    <<        return to mem 1
    end else

<] while running, loop back
