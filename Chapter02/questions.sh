#! /bin/sh

# Ask some questions and collect the answer

# Display a welcome box
dialog --title "Questionnaire" --msgbox "Welcome to my simple survey" 9 18

# Display a Confirm box, and let user to choose whether he'd like to play
dialog --title "Confirm" --yesno "Are you willing to take part?" 9 18

# Check the quit status of the last command, if it's not zero, then it mens the user doesn't want to play, so just clear up, and quit
if [ $? != 0 ]
then
	dialog --infobox "Thank you anyway" 5 20
	sleep 2
	dialog --clear
	exit 0
fi

# Display a inputbox, and let user enter his name, then redirect it's input to a tmp file
dialog --title "Questionnaire" --inputbox "Please enter your name" 9 30 2> _1.txt

# Get user's name from tmp file, and give it to a variable named Q_NAME
Q_NAME=$(cat _1.txt)

# Display a menu, and let user choose his favoritue music' type
dialog --menu "$Q_NAME, what music do you like best?" 15 30 4 1 "Classical" 2 "Jazz" 3 "Country" 4 "Other" 2>_1.txt

# just like above, get user's answer from tmp file
Q_MUSIC=$(cat _1.txt)


# Check user's choice of music, and test whether it is Classical
if [ "$Q_MUSIC" = "1" ]
then
	dialog --title "like Classical" --msgbox "Good choice!" 12 25
else
	dialog --title "Doesn't like Classical" --msgbox "Shame" 12 25
fi


# Finally, clear up and exit
sleep 2
dialog --clear
exit 0
