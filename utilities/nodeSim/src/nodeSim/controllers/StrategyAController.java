/**
 * 
 */
package nodeSim.controllers;

import nodeSim.SimulationController;
import nodeSim.containers.Input;

/**
 * This is strategy A as described in the thesis.
 *
 */
public class StrategyAController extends SimulationController {

	public StrategyAController(Input input) {
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

		//move source to dest until the distance between the two nodes is 1
		while( Math.abs(posS-posD) > 1 ) {
			posS = moveNode(posS, dir);
		}
	}

	@Override
	public String getName() {
		return "StrategyA";
	}

}
