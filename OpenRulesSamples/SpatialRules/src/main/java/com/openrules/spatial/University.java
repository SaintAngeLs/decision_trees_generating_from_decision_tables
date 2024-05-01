package com.openrules.spatial;

import java.util.Arrays;

public class University extends GeoEntity {
	
	
	
	private Facility[] facilities = null;
	
	public University() {
		super();
	}

	@Override
	public String getType() {
		return "University";
	}

	public Facility[] getFacilities() {
		return facilities;
	}

	public void setFacilities(Facility[] facilities) {
		this.facilities = facilities;
	}
	
	public boolean includes(Facility facility) {
		if (facilities==null)
			return false;
		for (int i = 0; i < facilities.length; i++) {
			if (facilities[i].equals(facility))
				return true;
		}
		return false;
	}

	@Override
	public String toString() {
		return "University [facilities=" + Arrays.toString(facilities)
				+ ", getId()=" + getId() + ", getName()=" + getName()
				+ ", isSpatial()=" + isSpatial() 
				+ ", numberOfFacilities=" + ((facilities==null)? 0 : facilities.length)
				+ "]";
	}

}
