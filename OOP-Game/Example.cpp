
float applyPhysics(float vel, float accel, float drag, float max) {
	if (accel == 0) {
		if (abs(drag) > abs(vel)) {
			vel = 0;
		} else if (vel > 0) {
			vel -= drag;
		} else {
			vel += drag;
		}
	} else {
		vel += accel;
		
		if (vel > abs(max)) {
			vel = abs(max);
		} else if(vel < -abs(max)) {
			vel = -abs(max);
		}
	}

	return vel;
}