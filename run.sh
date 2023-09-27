#!/bin/bash

make re > /dev/null 2>&1
make clean > /dev/null 2>&1
rm *.log 2>&1 > /dev/null 2>&1

# Ask for the number of teams
while true; do
    read -p "How many teams should compete (2-9)? " NB_TEAMS
    if [[ $NB_TEAMS -ge 2 && $NB_TEAMS -le 9 ]]; then
        break
    else
        echo "Please enter a number between 2 and 9."
    fi
done

# Ask for the number of players
while true; do
    read -p "How many players (at least $(($NB_TEAMS + 1)))? " NB_PLAYERS
    if [[ $NB_PLAYERS -ge $(($NB_TEAMS + 1)) ]]; then
        break
    else
        echo "The number of players must be at least equal to the number of teams + 1."
    fi
done

# Ask whether to generate logs
while true; do
    read -p "Logs or Graphic version (L / G)? " VERSION
    if [[ $VERSION == "L" || $VERSION == "G" ]]; then
        break
    else
        echo "Please choose between Logs (L) or Graphic (G)."
    fi
done

# Launching the first two players
for team in 1 2; do
    if [[ $VERSION == "L" ]]; then
        ./lemipc $team >> ./output_$team.log 2>&1 &
    elif [[ $VERSION == "G" ]]; then
        ./lemipc $team 2>&1 &
    fi
done

# Launching the rest of the players
for ((i=3; i<=NB_PLAYERS; i++))
do
    RANDOM_NUMBER=$(( ( RANDOM % $NB_TEAMS )  + 1 ))
    sleep 0.5

    if [[ $VERSION == "L" ]]; then
        ./lemipc $RANDOM_NUMBER >> ./output_$i.log 2>&1 &
    elif [[ $VERSION == "G" ]]; then
        ./lemipc $RANDOM_NUMBER 2>&1 &
    fi
done

wait

if [[ $VERSION == "L" ]]; then
    for i in $(seq 1 $NB_PLAYERS); do
        sed -i 's/\x1B\[[0-9;]*[a-zA-Z]//g' output_$i.log
    done

    total_lines=$(cat *.log | wc -l)
    results=$((3 * $NB_PLAYERS))
    moves_played=$(( (total_lines - results ) / 40 ))

    winning_team=""
    for i in $(seq 1 $NB_PLAYERS); do
        if grep -q "|   YOUR TEAM WINS !    |" output_$i.log; then
            winning_team=$(grep -B 15 "|   YOUR TEAM WINS !    |" output_$i.log | grep -Eo '[1-9]' | head -1)
            break
        fi
    done


    echo "Total number of moves played: $moves_played"
    echo "Average number of moves played per player: $((moves_played / $NB_PLAYERS))"
    echo "Winning team: $winning_team"
fi
