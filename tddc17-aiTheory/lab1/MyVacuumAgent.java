
	    // State update based on the percept value and the last action
	    state.updatePosition((DynamicPercept)percept);
	    if (bump) {
			switch (state.agent_direction) {
			case MyAgentState.NORTH:
				state.updateWorld(state.agent_x_position,state.agent_y_position-1,state.WALL);
				break;
			case MyAgentState.EAST:
				state.updateWorld(state.agent_x_position+1,state.agent_y_position,state.WALL);
				break;
			case MyAgentState.SOUTH:
				state.updateWorld(state.agent_x_position,state.agent_y_position+1,state.WALL);
				break;
			case MyAgentState.WEST:
				state.updateWorld(state.agent_x_position-1,state.agent_y_position,state.WALL);
				break;
			}
	    }
	    if (dirt)
	    	state.updateWorld(state.agent_x_position,state.agent_y_position,state.DIRT);
	    else
	    	state.updateWorld(state.agent_x_position,state.agent_y_position,state.CLEAR);

	    state.printWorldDebug();

		// Look at our world view to find obstacles surrounding the agent
		int obstacleForward = -1;
		int obstacleLeft = -1;
		int obstacleRight = -1;
		switch (state.agent_direction) {
			case MyAgentState.NORTH:
				obstacleForward = state.world[state.agent_x_position][state.agent_y_position-1];
				obstacleLeft = state.world[state.agent_x_position-1][state.agent_y_position];
				obstacleRight = state.world[state.agent_x_position+1][state.agent_y_position];
				break;
			case MyAgentState.EAST:
				obstacleForward = state.world[state.agent_x_position+1][state.agent_y_position];
				obstacleLeft = state.world[state.agent_x_position][state.agent_y_position-1];
				obstacleRight = state.world[state.agent_x_position][state.agent_y_position+1];
				break;
			case MyAgentState.SOUTH:
				obstacleForward = state.world[state.agent_x_position][state.agent_y_position+1];
				obstacleLeft = state.world[state.agent_x_position+1][state.agent_y_position];
				obstacleRight = state.world[state.agent_x_position-1][state.agent_y_position];
				break;
			case MyAgentState.WEST:
				obstacleForward = state.world[state.agent_x_position-1][state.agent_y_position];
				obstacleLeft = state.world[state.agent_x_position][state.agent_y_position+1];
				obstacleRight = state.world[state.agent_x_position][state.agent_y_position-1];
				break;
		}

		// Get move in move_queue (-1 if no move was found)
		Integer temp = move_queue.poll();
		int move = temp != null ? temp : -1;

		// If we bump the last move did not change our position
		// so we remove it from the stack
		if (bump) {
			moves.pop();
		}
		if (allCleaned && home) {
			return NoOpAction.NO_OP; // all done!
		}

	    // Next action selection based on the percept value
	    if (dirt) {
	    	System.out.println("DIRT -> choosing SUCK action!");
	    	state.agent_last_action = state.ACTION_SUCK;
	    	return LIUVacuumEnvironment.ACTION_SUCK;
		} else if (move == -1) {
			if (obstacleForward == state.UNKNOWN || obstacleForward == state.HOME){
				if (backwards) {
					int previous_move = moves.pop();
					if (previous_move == MyAgentProgram.TURN_RIGHT) {
						move = MyAgentProgram.TURN_LEFT;
					} else {
						move = MyAgentProgram.TURN_RIGHT;
					}
					moves.push(move);
					backwards = false;
				}
				move = MyAgentProgram.MOVE_FORWARD;
				moves.push(move);
			} else if (obstacleRight == state.UNKNOWN) {
				move = MyAgentProgram.TURN_RIGHT;
				if (backwards) {
					moves.push(MyAgentProgram.TURN_LEFT);
					backwards = false;
				} else {
					moves.push(move);
				}
			} else if (obstacleLeft == state.UNKNOWN) {
				move = MyAgentProgram.TURN_LEFT;
				if (backwards) {
					moves.push(MyAgentProgram.TURN_RIGHT);
					backwards = false;
				} else {
					moves.push(move);
				}
			} else {
				int previous_move = moves.pop();
				if (previous_move == MyAgentProgram.MOVE_FORWARD){
					if (!backwards){
						move = MyAgentProgram.TURN_RIGHT;
						move_queue.add(MyAgentProgram.TURN_RIGHT);
						move_queue.add(MyAgentProgram.MOVE_FORWARD);
						backwards = true;
					} else {
						move = MyAgentProgram.MOVE_FORWARD;
					}
				} else if (previous_move == MyAgentProgram.TURN_RIGHT) {
					move = MyAgentProgram.TURN_LEFT;
				} else {
					move = MyAgentProgram.TURN_RIGHT;
				}
			}
	    }

		// Reset world and re-use dfs to find home
		if (moves.empty()) {
			allCleaned = true;
			backwards = false;
			for (int i = 0; i < 30; i++) {
				for (int j = 0; j < 30; j++) {
					if (state.world[i][j] == state.CLEAR) {
						state.world[i][j] = state.UNKNOWN;
					}
				}
			}
		}

		// Perform the given move
		switch (move) {
			case MyAgentProgram.MOVE_FORWARD:
				state.agent_last_action = state.ACTION_MOVE_FORWARD;
				return LIUVacuumEnvironment.ACTION_MOVE_FORWARD;
			case MyAgentProgram.TURN_LEFT:
				state.agent_direction = ((state.agent_direction - 1) % 4);
				if (state.agent_direction < 0)
					state.agent_direction += 4;
				state.agent_last_action = state.ACTION_TURN_LEFT;
				return LIUVacuumEnvironment.ACTION_TURN_LEFT;
			case MyAgentProgram.TURN_RIGHT:
				state.agent_direction = ((state.agent_direction + 1) % 4);
				state.agent_last_action = state.ACTION_TURN_RIGHT;
				return LIUVacuumEnvironment.ACTION_TURN_RIGHT;
			default:
				return NoOpAction.NO_OP;
		}
	}
}
