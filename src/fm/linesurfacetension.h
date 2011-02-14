/*
 *
 *                 #####    #####   ######  ######  ###   ###
 *               ##   ##  ##   ##  ##      ##      ## ### ##
 *              ##   ##  ##   ##  ####    ####    ##  #  ##
 *             ##   ##  ##   ##  ##      ##      ##     ##
 *            ##   ##  ##   ##  ##      ##      ##     ##
 *            #####    #####   ##      ######  ##     ##
 *
 *
 *             OOFEM : Object Oriented Finite Element Code
 *
 *               Copyright (C) 1993 - 2011   Borek Patzak
 *
 *
 *
 *       Czech Technical University, Faculty of Civil Engineering,
 *   Department of Structural Mechanics, 166 29 Prague, Czech Republic
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef linesurfacetension_h
#define linesurfacetension_h

#include "fmelement.h"
#include "structuralelement.h"
#include "domain.h"

#include "spatiallocalizer.h"
#include "eleminterpmapperinterface.h"

namespace oofem {

/**
 * Implements the load and tangent for surface tension boundary potential.
 * This class is a base class for higher order and specifically introduces simplifications for linear elements, and components are exact.
 * The derivations are to long to write here, see documentation for further detail.
 *
 * This class could serve as a reference for the math if these parts were to ever be moved towards the boundary conditions.
 * I'm not convinced that it belongs to a boundary condition class.
 * The load is dependent on the solution, thus there is a "stiffness"/tangent matrix as well.
 * @author Mikael Öhman
 */
class LineSurfaceTension : 
	public FMElement, 
	public SpatialLocalizerInterface, 
	public EIPrimaryUnknownMapperInterface
{
protected:
	/// Material parameter for the surface energy. Defines the traction as @f$ \mathbf{t}_s = 2\kappa\gamma_s \mathbf{n} @f$.
	double gamma_s;
	/// Flags denoting if a a node is on the boundary.
	bool bflag1, bflag2;
	/// True if used as FMElement
	bool fmtype;

public:
    /**
     * Constructor. Creates an element with number n belonging to domain aDomain.
     * @param n Element's number
     * @param d Pointer to the domain to which element belongs.
     */
	LineSurfaceTension (int n, Domain *d);
    /// Destructor.
	~LineSurfaceTension ();

	/**
	 * Initializes the element.
	 * Reads
	 * - gamma_s (required)
	 */
	IRResultType initializeFrom(InputRecord *);

	virtual void giveCharacteristicVector(FloatArray &answer, CharType type, ValueModeType mode, TimeStep *tStep);
	virtual void giveCharacteristicMatrix(FloatMatrix &answer, CharType type, TimeStep *tStep);

	/**
	 * Computes the load vector.
	 */
	virtual void computeLoadVector(FloatArray &answer, ValueModeType mode, TimeStep *tStep);

	/**
	 * Computes tangent to load vector.
	 * @param answer
	 */
	virtual void computeTangent(FloatMatrix &answer, TimeStep *tStep);
	
	virtual void computeN(FloatArray &answer, const FloatArray &lcoords) const;

    virtual int computeGlobalCoordinates(FloatArray &answer, const FloatArray &lcoords);
    virtual int computeLocalCoordinates(FloatArray &answer, const FloatArray &gcoords);

	virtual Element_Geometry_Type giveGeometryType() const { return EGT_line_1; }
	virtual int computeNumberOfDofs(EquationID ut) { return ut == EID_MomentumBalance || ut == EID_MomentumBalance_ConservationEquation ? 4 : 0;}
    virtual void giveDofManDofIDMask(int inode, EquationID ut, IntArray &answer) const;

    Interface *giveInterface(InterfaceType it);

    // SpatialLocalizer Interface:
    virtual Element *SpatialLocalizerI_giveElement() { return this; }
    virtual int SpatialLocalizerI_containsPoint(const FloatArray &coords) { return false; }
    virtual double SpatialLocalizerI_giveDistanceFromParametricCenter(const FloatArray &gcoords);

    virtual int EIPrimaryUnknownMI_computePrimaryUnknownVectorAt(ValueModeType mode,
                                                                 TimeStep *tStep, const FloatArray &gcoords,
                                                                 FloatArray &answer);
    virtual void EIPrimaryUnknownMI_computePrimaryUnknownVectorAtLocal(ValueModeType mode,
                                                                 TimeStep *tStep, const FloatArray &gcoords,
                                                                 FloatArray &answer);
    virtual void EIPrimaryUnknownMI_givePrimaryUnknownVectorDofID(IntArray &answer);

    const char *giveClassName() const { return "LineSurfaceTension"; }
    classType giveClassID() const { return FMElementClass; } // TODO
};

} // end namespace oofem

#endif // surfacetension2d_h
