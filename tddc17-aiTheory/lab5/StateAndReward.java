public class StateAndReward {

	/* State discretization function for the angle controller */
	public static String getStateAngle(double angle, double vx, double vy) {
		if (angle > -0.25 && angle < 0.25)
			return "up";
		if (angle > 0.25 && angle < 1)
			return "right";
		if (angle > 1 && angle < 3.2)
			return "hard_right";
		if (angle < -0.25 && angle > -1)
			return "left";
		if (angle < -1 && angle > -3.2)
			return "hard_left";

		return "problem...";
	}

	/* Reward function for the angle controller */
	public static double getRewardAngle(double angle, double vx, double vy) {
		if (angle > -0.25 && angle < 0.25)
			return 1;
		if (angle > 0.25 && angle < 1)
			return -1;
		if (angle > 1 && angle < 3.2)
			return -5;
		if (angle < -0.25 && angle > -1)
			return -1;
		if (angle < -1 && angle > -3.2)
			return -5;
		return 0;
	}

	/* State discretization function for the full hover controller */
	public static String getStateHover(double angle, double vx, double vy) {
		int angleDiscrete = discretize2(angle, 8, -Math.PI / 4, Math.PI / 4);
		int vxDiscrete	= discretize2(vx, 5, -1, 1);
		int vyDiscrete = discretize2(vy, 5, -1, 1);

		return angleDiscrete + "." + vxDiscrete + "." + vyDiscrete;
	}

	/* Reward function for the full hover controller */
	public static double getRewardHover(double angle, double vx, double vy) {
		double angleReward = Math.min(Math.sqrt(1 / (Math.abs(angle))), 20);
		double vxReward = -Math.abs(vx);
		double vyReward = -Math.abs(vy) * 3;

		return angleReward + vxReward + vyReward;
	}


}
