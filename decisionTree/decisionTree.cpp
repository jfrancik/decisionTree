/* The basic decision tree demo
 * Jarek Francik, Kingston University 2019
 *
 * Inspired the Artificial Intelligence for Games system.
 * Portions Copyright (c) Ian Millington 2003-2006. All Rights Reserved.
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string>

using namespace std;

///////////////////////////////////////////////////////////////
// Abstract Base Classes

class DTNode;
class DTAction;

// Abstract class: The base for all Decision Tree Nodes
class DTNode
{
public:
	// This pure virtual function (with no implementation)
	// makes a decision and returns the next node down the tree.
	virtual DTAction* makeDecision() = 0;
};

// The abstract class for the Decision node - has a "yes" branch and a "no" branch
class DTDecision : public DTNode
{
public:
	DTNode* branchYes;
	DTNode* branchNo;

	// This pure virtual function should be replaced with the actual decision-making code
	// return true to select branchTrue or false to select branchFalse
	virtual bool getBranch() = 0;

	// The implementation of the decision making algorithm...
	virtual DTAction* makeDecision()
	{
		// the next branch (either yes or no) is taken on the basis of the getBranch result...
		DTNode* nextBranch = getBranch() ? branchYes : branchNo;

		// if the branch exists, recursively search down the tree...
		if (nextBranch)
			return nextBranch->makeDecision();
		else
			return NULL;	// no decision if no branch...
	}
};

// The abstract class for the Actions
class DTAction : public DTNode
{
public:
	// This is the end of the decision process - so the function simply returns itself
	virtual DTAction* makeDecision()
	{
		return this;
	}

	// This pure virtual function should be rewritten with the actual action
	virtual void act() = 0;
};

///////////////////////////////////////////////////////////////
// Sample Decision Classes

// Random decision
class DTRandomDecision : public DTDecision
{
public:
	virtual bool getBranch()
	{
		return rand() % 2 == 0;
	}
};

// Decision made by asking the user

class DTInteractiveDecision : public DTDecision
{
public:
	string question;

	virtual bool getBranch()
	{
		cout << question << " (Y/N)";

		int c = _getch();
		cout << endl;

		return (toupper(c) == 'Y');
	}
};

// This very simple action just displays a text
class DTTextAction : public DTAction
{
public:
	string actionText;

	virtual void act()
	{
		cout << actionText << endl;
	}
};

// The main entry point, we set up our decision tree here, and then run it.
int main(int argc, char** argv)
{
	// Holds our list of decisions
	DTInteractiveDecision decisions[7];

	// Holds our list of actions
	DTTextAction actions[5];

	// Set up the tree
	decisions[0].question = "Q1. Can you see any enemies? (Yes->Q3, No->Q2)";
	decisions[0].branchYes = &decisions[2];
	decisions[0].branchNo = &decisions[1];

	decisions[1].question = "Q2. Is your health okay? (Yes->A1, No->A2)";
	decisions[1].branchYes = &actions[0];
	decisions[1].branchNo = &actions[1];

	decisions[2].question = "Q3. Is the enemy close by? (Yes->Q4, No->Q5)";
	decisions[2].branchYes = &decisions[3];
	decisions[2].branchNo = &decisions[4];

	decisions[3].question = "Q4. Is the enemy dangerous? (Yes->Q6, No->A3)";
	decisions[3].branchYes = &decisions[5];
	decisions[3].branchNo = &actions[2];

	decisions[4].question = "Q5. Is your health okay? (Yes->A3, No->A4)";
	decisions[4].branchYes = &actions[2];
	decisions[4].branchNo = &actions[3];

	decisions[5].question = "Q6. Are you in cover? (Yes->A3, No->Q7)";
	decisions[5].branchYes = &actions[2];
	decisions[5].branchNo = &decisions[6];

	decisions[6].question = "Q7. Is there cover nearby? (Yes->A5, No->A3)";
	decisions[6].branchYes = &actions[4];
	decisions[6].branchNo = &actions[2];

	actions[0].actionText = "A1. Seek out enemies.";
	actions[1].actionText = "A2. Seek out health packs.";
	actions[2].actionText = "A3. Attack enemy.";
	actions[3].actionText = "A4. Run away from enemy.";
	actions[4].actionText = "A5. Head for cover.";

	// Find the decision

	cout << "Decision Tree Demo (Ctrl+C to exit)" << endl;
	DTAction* pAction = decisions[0].makeDecision();
	if (pAction)
		pAction->act();

	return 0;
}
