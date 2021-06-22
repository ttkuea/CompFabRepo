//Computational Fabrication Assignment #1
// By David Levin 2014
#include <iostream>
#include <vector>
#include "../include/CompFab.h"
#include "../include/Mesh.h"
#include <math.h>

//Ray-Triangle Intersection
//Returns 1 if triangle and ray intersect, 0 otherwise
int rayTriangleIntersection(CompFab::Ray &ray, CompFab::Triangle &triangle)
{
    /********* ASSIGNMENT *********/
    /* Ray-Triangle intersection test: Return 1 if ray intersects triangle, 
     * 0 otherwise */
    
    //This code inspired from Möller–Trumbore intersection algorithm from wikipedia
    double kEpsilon = 0.0000001;
    CompFab::Vec3 edge1, edge2, h, s, q;
    double a, f, u, v;

    edge1 = triangle.m_v2 - triangle.m_v1;
    edge2 = triangle.m_v3 - triangle.m_v1;

    h = ray.m_direction % edge2;
    a = edge1 * h;

    if (a > -kEpsilon && a < kEpsilon) return 0;

    f = 1.0 / a;
    s = ray.m_origin - triangle.m_v1;
    u = f * (s * h);
    if (u < 0.0 || u > 1.0) return 0;

    q = s % edge1;
    v = f * (ray.m_direction * q);
    if (v < 0.0 || u + v > 1.0) return 0;

    double t = f * (edge2 * q);
    if (t > kEpsilon) return 1;
    return 0;
 
}

//Triangle list (global)
typedef std::vector<CompFab::Triangle> TriangleList;

TriangleList g_triangleList;
CompFab::VoxelGrid *g_voxelGrid;

//Number of intersections with surface made by a ray originating at voxel and cast in direction.
int numSurfaceIntersections(CompFab::Vec3 &voxelPos, CompFab::Vec3 &dir)
{
    
    unsigned int numHits = 0;
    
    /********* ASSIGNMENT *********/
    /* Check and return the number of times a ray cast in direction dir, 
     * from voxel center voxelPos intersects the surface */
    CompFab::RayStruct ray = CompFab::RayStruct(voxelPos, dir);
    for (int i = 0; i < g_triangleList.size(); i++) {
        CompFab::Triangle triangle = g_triangleList[i];
        if (rayTriangleIntersection(ray, triangle)) {
            numHits++;
        }
    }
    return numHits;
}

bool loadMesh(char *filename, unsigned int dim)
{
    g_triangleList.clear();
    
    Mesh *tempMesh = new Mesh(filename, true);
    
    CompFab::Vec3 v1, v2, v3;

    //copy triangles to global list
    for(unsigned int tri =0; tri<tempMesh->t.size(); ++tri)
    {
        v1 = tempMesh->v[tempMesh->t[tri][0]];
        v2 = tempMesh->v[tempMesh->t[tri][1]];
        v3 = tempMesh->v[tempMesh->t[tri][2]];
        g_triangleList.push_back(CompFab::Triangle(v1,v2,v3));
    }

    //Create Voxel Grid
    CompFab::Vec3 bbMax, bbMin;
    BBox(*tempMesh, bbMin, bbMax);
    
    //Build Voxel Grid
    double bbX = bbMax[0] - bbMin[0];
    double bbY = bbMax[1] - bbMin[1];
    double bbZ = bbMax[2] - bbMin[2];
    double spacing;
    
    if(bbX > bbY && bbX > bbZ)
    {
        spacing = bbX/(double)(dim-2);
    } else if(bbY > bbX && bbY > bbZ) {
        spacing = bbY/(double)(dim-2);
    } else {
        spacing = bbZ/(double)(dim-2);
    }
    
    CompFab::Vec3 hspacing(0.5*spacing, 0.5*spacing, 0.5*spacing);
    
    g_voxelGrid = new CompFab::VoxelGrid(bbMin-hspacing, dim, dim, dim, spacing);

    delete tempMesh;
    
    return true;
   
}

void saveVoxelsToObj(const char * outfile)
{
 
    Mesh box;
    Mesh mout;
    int nx = g_voxelGrid->m_dimX;
    int ny = g_voxelGrid->m_dimY;
    int nz = g_voxelGrid->m_dimZ;
    double spacing = g_voxelGrid->m_spacing;
    
    CompFab::Vec3 hspacing(0.5*spacing, 0.5*spacing, 0.5*spacing);
    
    for (int ii = 0; ii < nx; ii++) {
        for (int jj = 0; jj < ny; jj++) {
            for (int kk = 0; kk < nz; kk++) {
                if(!g_voxelGrid->isInside(ii,jj,kk)){
                    continue;
                }
                CompFab::Vec3 coord(0.5f + ((double)ii)*spacing, 0.5f + ((double)jj)*spacing, 0.5f+((double)kk)*spacing);
                CompFab::Vec3 box0 = coord - hspacing;
                CompFab::Vec3 box1 = coord + hspacing;
                makeCube(box, box0, box1);
                mout.append(box);
            }
        }
    }

    mout.save_obj(outfile);
}


int main(int argc, char **argv)
{

    unsigned int dim = 64; //dimension of voxel grid (e.g. 32x32x32)

    //Load OBJ
    if(argc < 3)
    {
        std::cout<<"Usage: Voxelizer InputMeshFilename OutputMeshFilename \n";
        exit(0);
    }
    
    std::cout<<"Load Mesh : "<<argv[1]<<"\n";
    loadMesh(argv[1], dim);
    

    
    //Cast ray, check if voxel is inside or outside
    //even number of surface intersections = outside (OUT then IN then OUT)
    // odd number = inside (IN then OUT)
    CompFab::Vec3 voxelPos;
    CompFab::Vec3 direction(1.0,0.0,0.0);
    
    /********* ASSIGNMENT *********/
    /* Iterate over all voxels in g_voxelGrid and test whether they are inside our outside of the
     * surface defined by the triangles in g_triangleList */
    int nx = g_voxelGrid->m_dimX;
    int ny = g_voxelGrid->m_dimY;
    int nz = g_voxelGrid->m_dimZ;
    double spacing = g_voxelGrid->m_spacing;
    CompFab::Vec3 leftPos = g_voxelGrid->m_lowerLeft;

    for (int ii = 0; ii < nx; ii++) {
        for (int jj = 0; jj < ny; jj++) {
            for (int kk = 0; kk < nz; kk++) {
                voxelPos = CompFab::Vec3(leftPos.m_x + ((double)ii) * spacing, leftPos.m_y + ((double)jj) * spacing, leftPos.m_z + ((double)kk) * spacing);
                if (numSurfaceIntersections(voxelPos, direction) % 2 != 0) {
                    //[k*(m_dimX*m_dimY)+j*m_dimY + i]
                    g_voxelGrid->isInside(ii, jj, kk) = true;
                }
            }
        }
    }
    
    //Write out voxel data as obj
    saveVoxelsToObj(argv[2]);
    
    delete g_voxelGrid;
}