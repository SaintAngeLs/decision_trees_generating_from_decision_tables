package com.openrules.spatial;

import java.util.Arrays;

public class Facility extends GeoEntity {
	
	public Facility() {
		super();
	}

	private Window[] windows = null;

	public Window[] getWindows() {
		return windows;
	}

	public void setWindows(Window[] windows) {
		this.windows = windows;
	}

	@Override
	public String toString() {
		return "Facility [windows=" + Arrays.toString(windows) + ", getId()="
				+ getId() + ", getName()=" + getName() + ", isSpatial()=" + isSpatial() + "]";
	}

	@Override
	public String getType() {
		return "Facility";
	}

}
