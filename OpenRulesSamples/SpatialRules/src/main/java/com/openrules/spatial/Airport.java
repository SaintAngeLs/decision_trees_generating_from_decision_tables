package com.openrules.spatial;

import com.vividsolutions.jts.geom.Geometry;

public class Airport extends GeoEntity {
	
	public Airport(int i, Geometry geometry) {
		super(i,geometry);
	}

	@Override
	public String getType() {
		return "Airport";
	}

	@Override
	public String toString() {
		return "Airport [getId()=" + getId() + ", getName()=" + getName()
				+ ", getGeometry()=" + getGeometry() + ", isSpatial()="
				+ isSpatial() + "]";
	}
	
	

}
