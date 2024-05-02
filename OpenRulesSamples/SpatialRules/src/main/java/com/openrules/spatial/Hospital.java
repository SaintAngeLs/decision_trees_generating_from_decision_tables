package com.openrules.spatial;

import com.vividsolutions.jts.geom.Geometry;

public class Hospital extends GeoEntity {
	
	public Hospital(int i, Geometry geometry) {
		super(i,geometry);
	}

	@Override
	public String getType() {
		return "Hospital";
	}

	@Override
	public String toString() {
		return "Hospital [getId()=" + getId() + ", getName()=" + getName()
				+ ", points="
				+ getGeometry().getNumPoints() + ", isSpatial()="
				+ isSpatial() 
				+ "]";
	}
	
	

}
