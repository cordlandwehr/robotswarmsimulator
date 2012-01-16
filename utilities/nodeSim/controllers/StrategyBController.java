/**
 * 
 */
package nodeSim.controllers;

import nodeSim.SimulationController;
import nodeSim.containers.Input;

/**
 * This is strategy B as described in the thesis.
 *
 */
public class StrategyBController extends SimulationController {

	public StrategyBController(Input input) {
		super(input);
	}

	@Override
	protected void rearrange(int posS, int posD) {
		int dir = 0;
		//if source is left to dest, then move the node to the right - else move it to the left 
		if (posS < posD)
			dir = 1;
		else
			dir = -1;

		//move by one position from source to dest
		posS = moveNode(posS, dir);
	}

	@Override
	public String getName() {
		return "StrategyB";
	}

}
