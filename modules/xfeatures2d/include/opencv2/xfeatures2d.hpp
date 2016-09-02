/*
By downloading, copying, installing or using the software you agree to this
license. If you do not agree to this license, do not download, install,
copy or use the software.

                          License Agreement
               For Open Source Computer Vision Library
                       (3-clause BSD License)

Copyright (C) 2013, OpenCV Foundation, all rights reserved.
Third party copyrights are property of their respective owners.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  * Neither the names of the copyright holders nor the names of the contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

This software is provided by the copyright holders and contributors "as is" and
any express or implied warranties, including, but not limited to, the implied
warranties of merchantability and fitness for a particular purpose are
disclaimed. In no event shall copyright holders or contributors be liable for
any direct, indirect, incidental, special, exemplary, or consequential damages
(including, but not limited to, procurement of substitute goods or services;
loss of use, data, or profits; or business interruption) however caused
and on any theory of liability, whether in contract, strict liability,
or tort (including negligence or otherwise) arising in any way out of
the use of this software, even if advised of the possibility of such damage.
*/

#ifndef __OPENCV_XFEATURES2D_HPP__
#define __OPENCV_XFEATURES2D_HPP__

#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"

/** @defgroup xfeatures2d Extra 2D Features Framework
@{
    @defgroup xfeatures2d_experiment Experimental 2D Features Algorithms

This section describes experimental algorithms for 2d feature detection.

    @defgroup xfeatures2d_nonfree Non-free 2D Features Algorithms

This section describes two popular algorithms for 2d feature detection, SIFT and SURF, that are
known to be patented. Use them at your own risk.

@}
*/

namespace cv
{
namespace xfeatures2d
{

//! @addtogroup xfeatures2d_experiment
//! @{

/** @brief Class implementing the FREAK (*Fast Retina Keypoint*) keypoint descriptor, described in @cite AOV12 .

The algorithm propose a novel keypoint descriptor inspired by the human visual system and more
precisely the retina, coined Fast Retina Key- point (FREAK). A cascade of binary strings is
computed by efficiently comparing image intensities over a retinal sampling pattern. FREAKs are in
general faster to compute with lower memory load and also more robust than SIFT, SURF or BRISK.
They are competitive alternatives to existing keypoints in particular for embedded applications.

@note
   -   An example on how to use the FREAK descriptor can be found at
        opencv_source_code/samples/cpp/freak_demo.cpp
 */
class CV_EXPORTS_W FREAK : public Feature2D
{
public:

    enum
    {
        NB_SCALES = 64, NB_PAIRS = 512, NB_ORIENPAIRS = 45
    };

    /**
    @param orientationNormalized Enable orientation normalization.
    @param scaleNormalized Enable scale normalization.
    @param patternScale Scaling of the description pattern.
    @param nOctaves Number of octaves covered by the detected keypoints.
    @param selectedPairs (Optional) user defined selected pairs indexes,
     */
    CV_WRAP static Ptr<FREAK> create(bool orientationNormalized = true,
                             bool scaleNormalized = true,
                             float patternScale = 22.0f,
                             int nOctaves = 4,
                             const std::vector<int>& selectedPairs = std::vector<int>());
};


/** @brief The class implements the keypoint detector introduced by @cite Agrawal08, synonym of StarDetector. :
 */
class CV_EXPORTS_W StarDetector : public Feature2D
{
public:
    //! the full constructor
    CV_WRAP static Ptr<StarDetector> create(int maxSize=45, int responseThreshold=30,
                         int lineThresholdProjected=10,
                         int lineThresholdBinarized=8,
                         int suppressNonmaxSize=5);
};

/*
 * BRIEF Descriptor
 */

/** @brief Class for computing BRIEF descriptors described in @cite calon2010 .

@param bytes legth of the descriptor in bytes, valid values are: 16, 32 (default) or 64 .
@param use_orientation sample patterns using keypoints orientation, disabled by default.

 */
class CV_EXPORTS_W BriefDescriptorExtractor : public Feature2D
{
public:
    CV_WRAP static Ptr<BriefDescriptorExtractor> create( int bytes = 32, bool use_orientation = false );
};

/** @brief Class implementing the locally uniform comparison image descriptor, described in @cite LUCID

An image descriptor that can be computed very fast, while being
about as robust as, for example, SURF or BRIEF.
 */
class CV_EXPORTS_W LUCID : public Feature2D
{
public:
    /**
     * @param lucid_kernel kernel for descriptor construction, where 1=3x3, 2=5x5, 3=7x7 and so forth
     * @param blur_kernel kernel for blurring image prior to descriptor construction, where 1=3x3, 2=5x5, 3=7x7 and so forth
     */
    CV_WRAP static Ptr<LUCID> create(const int lucid_kernel, const int blur_kernel);
};


/*
* LATCH Descriptor
*/

/** latch Class for computing the LATCH descriptor.
If you find this code useful, please add a reference to the following paper in your work:
Gil Levi and Tal Hassner, "LATCH: Learned Arrangements of Three Patch Codes", arXiv preprint arXiv:1501.03719, 15 Jan. 2015

LATCH is a binary descriptor based on learned comparisons of triplets of image patches.

* bytes is the size of the descriptor - can be 64, 32, 16, 8, 4, 2 or 1
* rotationInvariance - whether or not the descriptor should compansate for orientation changes.
* half_ssd_size - the size of half of the mini-patches size. For example, if we would like to compare triplets of patches of size 7x7x
    then the half_ssd_size should be (7-1)/2 = 3.

Note: the descriptor can be coupled with any keypoint extractor. The only demand is that if you use set rotationInvariance = True then
    you will have to use an extractor which estimates the patch orientation (in degrees). Examples for such extractors are ORB and SIFT.

Note: a complete example can be found under /samples/cpp/tutorial_code/xfeatures2D/latch_match.cpp

*/
class CV_EXPORTS_W LATCH : public Feature2D
{
public:
    CV_WRAP static Ptr<LATCH> create(int bytes = 32, bool rotationInvariance = true, int half_ssd_size=3);
};

/** @brief Class implementing DAISY descriptor, described in @cite Tola10

@param radius radius of the descriptor at the initial scale
@param q_radius amount of radial range division quantity
@param q_theta amount of angular range division quantity
@param q_hist amount of gradient orientations range division quantity
@param norm choose descriptors normalization type, where
DAISY::NRM_NONE will not do any normalization (default),
DAISY::NRM_PARTIAL mean that histograms are normalized independently for L2 norm equal to 1.0,
DAISY::NRM_FULL mean that descriptors are normalized for L2 norm equal to 1.0,
DAISY::NRM_SIFT mean that descriptors are normalized for L2 norm equal to 1.0 but no individual one is bigger than 0.154 as in SIFT
@param H optional 3x3 homography matrix used to warp the grid of daisy but sampling keypoints remains unwarped on image
@param interpolation switch to disable interpolation for speed improvement at minor quality loss
@param use_orientation sample patterns using keypoints orientation, disabled by default.

 */
class CV_EXPORTS_W DAISY : public Feature2D
{
public:
    enum
    {
        NRM_NONE = 100, NRM_PARTIAL = 101, NRM_FULL = 102, NRM_SIFT = 103,
    };
    CV_WRAP static Ptr<DAISY> create( float radius = 15, int q_radius = 3, int q_theta = 8,
                int q_hist = 8, int norm = DAISY::NRM_NONE, InputArray H = noArray(),
                bool interpolation = true, bool use_orientation = false );

    /** @overload
     * @param image image to extract descriptors
     * @param keypoints of interest within image
     * @param descriptors resulted descriptors array
     */
    virtual void compute( InputArray image, std::vector<KeyPoint>& keypoints, OutputArray descriptors ) = 0;

    virtual void compute( InputArrayOfArrays images,
                          std::vector<std::vector<KeyPoint> >& keypoints,
                          OutputArrayOfArrays descriptors );

    /** @overload
     * @param image image to extract descriptors
     * @param roi region of interest within image
     * @param descriptors resulted descriptors array for roi image pixels
     */
    virtual void compute( InputArray image, Rect roi, OutputArray descriptors ) = 0;

    /**@overload
     * @param image image to extract descriptors
     * @param descriptors resulted descriptors array for all image pixels
     */
    virtual void compute( InputArray image, OutputArray descriptors ) = 0;

    /**
     * @param y position y on image
     * @param x position x on image
     * @param orientation orientation on image (0->360)
     * @param descriptor supplied array for descriptor storage
     */
    virtual void GetDescriptor( double y, double x, int orientation, float* descriptor ) const = 0;

    /**
     * @param y position y on image
     * @param x position x on image
     * @param orientation orientation on image (0->360)
     * @param descriptor supplied array for descriptor storage
     * @param H homography matrix for warped grid
     */
    virtual bool GetDescriptor( double y, double x, int orientation, float* descriptor, double* H ) const = 0;

    /**
     * @param y position y on image
     * @param x position x on image
     * @param orientation orientation on image (0->360)
     * @param descriptor supplied array for descriptor storage
     */
    virtual void GetUnnormalizedDescriptor( double y, double x, int orientation, float* descriptor ) const = 0;

    /**
     * @param y position y on image
     * @param x position x on image
     * @param orientation orientation on image (0->360)
     * @param descriptor supplied array for descriptor storage
     * @param H homography matrix for warped grid
     */
    virtual bool GetUnnormalizedDescriptor( double y, double x, int orientation, float* descriptor , double *H ) const = 0;

};

/** @brief Class implementing the MSD (*Maximal Self-Dissimilarity*) keypoint detector, described in @cite Tombari14.

The algorithm implements a novel interest point detector stemming from the intuition that image patches
which are highly dissimilar over a relatively large extent of their surroundings hold the property of
being repeatable and distinctive. This concept of "contextual self-dissimilarity" reverses the key
paradigm of recent successful techniques such as the Local Self-Similarity descriptor and the Non-Local
Means filter, which build upon the presence of similar - rather than dissimilar - patches. Moreover,
it extends to contextual information the local self-dissimilarity notion embedded in established
detectors of corner-like interest points, thereby achieving enhanced repeatability, distinctiveness and
localization accuracy.

*/

class CV_EXPORTS_W MSDDetector : public Feature2D {

public:

    static Ptr<MSDDetector> create(int m_patch_radius = 3, int m_search_area_radius = 5,
            int m_nms_radius = 5, int m_nms_scale_radius = 0, float m_th_saliency = 250.0f, int m_kNN = 4,
            float m_scale_factor = 1.25f, int m_n_scales = -1, bool m_compute_orientation = false);
};


/*
* Position-Color-Texture signatures
*/

namespace pct_signatures
{
    /**
    * @brief Lp distance function selector.
    */
    CV_WRAP enum DistanceFunction
    {
        L0_25, L0_5, L1, L2, L2SQUARED, L5, L_INFINITY
    };

    /**
    * @brief Point distributions supported by random point generator.
    */
    CV_WRAP enum PointDistribution {
        UNIFORM,    //!< Generate numbers uniformly.
        REGULAR,    //!< Generate points in a regular grid.
        NORMAL      //!< Generate points with normal (gaussian) distribution.
    };

    /**
    * @brief Similarity function selector.
    * @see
    *       Christian Beecks, Merih Seran Uysal, Thomas Seidl.
    *       Signature quadratic form distance.
    *       In Proceedings of the ACM International Conference on Image and Video Retrieval, pages 438-445.
    *       ACM, 2010.
    * @cite BeecksUS10
    * @note For selected distance function \f[ d(c_i, c_j) \f]  and parameter \f[ \alpha \f]:
    */
    CV_WRAP enum SimilarityFunction
    {
        MINUS,      //!< \f[ -d(c_i, c_j) \f]
        GAUSSIAN,   //!< \f[ e^{ -\alpha * d^2(c_i, c_j)} \f]
        HEURISTIC   //!< \f[ \frac{1}{\alpha + d(c_i, c_j)} \f]
    };


}

/**
* @brief Class implementing PCT (position-color-texture) signature extraction
*       as described in @cite KrulisLS16.
*       The algorithm is divided to a feature sampler and a clusterizer.
*       Feature sampler produces samples at given set of coordinates.
*       Clusterizer then produces clusters of these samples using k-means algorithm.
*       Resulting set of clusters is the signature of the input image.
*
*       A signature is an array of SIGNATURE_DIMENSION-dimensional points.
*       Used dimensions are:
*       weight, x, y position; lab color, contrast, entropy.
* @cite KrulisLS16
* @cite BeecksUS10
*/
class CV_EXPORTS_W PCTSignatures : public Algorithm
{
public:

    /**
    * @brief Creates PCTSignatures algorithm using sample and seed count.
    *       It generates its own sets of sampling points and clusterization seed indexes.
    * @param initSampleCount Number of points used for image sampling.
    * @param initSeedCount Number of initial clusterization seeds.
    *       Must be lower or equal to initSampleCount
    * @param pointDistribution Distribution of generated points.
    * @return Created algorithm.
    */
    CV_WRAP static Ptr<PCTSignatures> create(
        const int initSampleCount = 2000,
        const int initSeedCount = 400,
        const pct_signatures::PointDistribution pointDistribution = pct_signatures::PointDistribution::UNIFORM);

    /**
    * @brief Creates PCTSignatures algorithm using pre-generated sampling points
    *       and number of clusterization seeds. It uses the provided
    *       sampling points and generates its own clusterization seed indexes.
    * @param initSamplingPoints Sampling points used in image sampling.
    * @param initSeedCount Number of initial clusterization seeds.
    *       Must be lower or equal to initSamplingPoints.size().
    * @return Created algorithm.
    */
    CV_WRAP static Ptr<PCTSignatures> create(
        const std::vector<Point2f>& initSamplingPoints,
        const int initSeedCount);

    /**
    * @brief Creates PCTSignatures algorithm using pre-generated sampling points
    *       and clusterization seeds indexes.
    * @param initSamplingPoints Sampling points used in image sampling.
    * @param initClusterSeedIndexes Indexes of initial clusterization seeds.
    *       Its size must be lower or equal to initSamplingPoints.size().
    * @return Created algorithm.
    */
    CV_WRAP static Ptr<PCTSignatures> create(
        const std::vector<Point2f>& initSamplingPoints,
        const std::vector<int>& initClusterSeedIndexes);



    /**
    * @brief Computes signature of given image.
    * @param image Input image of CV_8U type.
    * @param signature Output computed signature.
    */
    CV_WRAP virtual void computeSignature(
        InputArray image,
        OutputArray signature) const = 0;

    /**
    * @brief Computes signatures for multiple images in parallel.
    * @param images Vector of input images of CV_8U type.
    * @param signatures Vector of computed signatures.
    */
    CV_WRAP virtual void computeSignatures(
        const std::vector<Mat>& images,
        std::vector<Mat>& signatures) const = 0;

    /**
    * @brief Draws signature in the source image and outputs the result.
    *       Signatures are visualized as a circle
    *       with radius based on signature weight
    *       and color based on signature color.
    *       Contrast and entropy are not visualized.
    * @param source Source image.
    * @param signature Image signature.
    * @param result Output result.
    * @param radiusToShorterSideRatio Determines maximal radius of signature in the output image.
    * @param borderThickness Border thickness of the visualized signature.
    */
    CV_WRAP static void drawSignature(
        const InputArray source,
        const InputArray signature,
        OutputArray result,
        float radiusToShorterSideRatio = 1.0 / 8,
        int borderThickness = 1);

    /**
    * @brief Generates initial sampling points according to selected point distribution.
    * @param initPoints Output vector where the generated points will be saved.
    * @param count Number of points to generate.
    * @param pointsDistribution Point distribution selector.
    * @note Generated coordinates are in range [0..1)
    */
    CV_WRAP static void generateInitPoints(
        std::vector<Point2f>& initPoints,
        const int count,
        pct_signatures::PointDistribution pointsDistribution);


    /**** sampler ****/

    /**
    * @brief Number of initial samples taken from the image.
    */
    CV_WRAP virtual int getSampleCount() const = 0;

    /**
    * @brief Color resolution of the greyscale bitmap represented in allocated bits
    *       (i.e., value 4 means that 16 shades of grey are used).
    *       The greyscale bitmap is used for computing contrast and entropy values.
    */
    CV_WRAP virtual int getGrayscaleBits() const = 0;
    /**
    * @brief Color resolution of the greyscale bitmap represented in allocated bits
    *       (i.e., value 4 means that 16 shades of grey are used).
    *       The greyscale bitmap is used for computing contrast and entropy values.
    */
    CV_WRAP virtual void setGrayscaleBits(int grayscaleBits) = 0;

    /**
    * @brief Size of the texture sampling window used to compute contrast and entropy
    *       (center of the window is always in the pixel selected by x,y coordinates
    *       of the corresponding feature sample).
    */
    CV_WRAP virtual int getWindowRadius() const = 0;
    /**
    * @brief Size of the texture sampling window used to compute contrast and entropy
    *       (center of the window is always in the pixel selected by x,y coordinates
    *       of the corresponding feature sample).
    */
    CV_WRAP virtual void setWindowRadius(int radius) = 0;


    /**
    * @brief Weights (multiplicative constants) that linearly stretch individual axes of the feature space
    *       (x,y = position; L,a,b = color in CIE Lab space; c = contrast. e = entropy)
    */
    CV_WRAP virtual float getWeightX() const = 0;
    /**
    * @brief Weights (multiplicative constants) that linearly stretch individual axes of the feature space
    *       (x,y = position; L,a,b = color in CIE Lab space; c = contrast. e = entropy)
    */
    CV_WRAP virtual void setWeightX(float weight) = 0;

    /**
    * @brief Weights (multiplicative constants) that linearly stretch individual axes of the feature space
    *       (x,y = position; L,a,b = color in CIE Lab space; c = contrast. e = entropy)
    */
    CV_WRAP virtual float getWeightY() const = 0;
    /**
    * @brief Weights (multiplicative constants) that linearly stretch individual axes of the feature space
    *       (x,y = position; L,a,b = color in CIE Lab space; c = contrast. e = entropy)
    */
    CV_WRAP virtual void setWeightY(float weight) = 0;

    /**
    * @brief Weights (multiplicative constants) that linearly stretch individual axes of the feature space
    *       (x,y = position; L,a,b = color in CIE Lab space; c = contrast. e = entropy)
    */
    CV_WRAP virtual float getWeightL() const = 0;
    /**
    * @brief Weights (multiplicative constants) that linearly stretch individual axes of the feature space
    *       (x,y = position; L,a,b = color in CIE Lab space; c = contrast. e = entropy)
    */
    CV_WRAP virtual void setWeightL(float weight) = 0;

    /**
    * @brief Weights (multiplicative constants) that linearly stretch individual axes of the feature space
    *       (x,y = position; L,a,b = color in CIE Lab space; c = contrast. e = entropy)
    */
    CV_WRAP virtual float getWeightA() const = 0;
    /**
    * @brief Weights (multiplicative constants) that linearly stretch individual axes of the feature space
    *       (x,y = position; L,a,b = color in CIE Lab space; c = contrast. e = entropy)
    */
    CV_WRAP virtual void setWeightA(float weight) = 0;

    /**
    * @brief Weights (multiplicative constants) that linearly stretch individual axes of the feature space
    *       (x,y = position; L,a,b = color in CIE Lab space; c = contrast. e = entropy)
    */
    CV_WRAP virtual float getWeightB() const = 0;
    /**
    * @brief Weights (multiplicative constants) that linearly stretch individual axes of the feature space
    *       (x,y = position; L,a,b = color in CIE Lab space; c = contrast. e = entropy)
    */
    CV_WRAP virtual void setWeightB(float weight) = 0;

    /**
    * @brief Weights (multiplicative constants) that linearly stretch individual axes of the feature space
    *       (x,y = position; L,a,b = color in CIE Lab space; c = contrast. e = entropy)
    */
    CV_WRAP virtual float getWeightConstrast() const = 0;
    /**
    * @brief Weights (multiplicative constants) that linearly stretch individual axes of the feature space
    *       (x,y = position; L,a,b = color in CIE Lab space; c = contrast. e = entropy)
    */
    CV_WRAP virtual void setWeightContrast(float weight) = 0;

    /**
    * @brief Weights (multiplicative constants) that linearly stretch individual axes of the feature space
    *       (x,y = position; L,a,b = color in CIE Lab space; c = contrast. e = entropy)
    */
    CV_WRAP virtual float getWeightEntropy() const = 0;
    /**
    * @brief Weights (multiplicative constants) that linearly stretch individual axes of the feature space
    *       (x,y = position; L,a,b = color in CIE Lab space; c = contrast. e = entropy)
    */
    CV_WRAP virtual void setWeightEntropy(float weight) = 0;

    /**
    * @brief Initial samples taken from the image.
    *       These sampled features become the input for clustering.
    */
    CV_WRAP virtual std::vector<Point2f> getSamplingPoints() const = 0;



    /**
    * @brief Weights (multiplicative constants) that linearly stretch individual axes of the feature space.
    * @param idx ID of the weight
    * @param value Value of the weight
    * @note
    *       WEIGHT_IDX = 0;
    *       X_IDX = 1;
    *       Y_IDX = 2;
    *       L_IDX = 3;
    *       A_IDX = 4;
    *       B_IDX = 5;
    *       CONTRAST_IDX = 6;
    *       ENTROPY_IDX = 7;
    */
    CV_WRAP virtual void setWeight(int idx, float value) = 0;
    /**
    * @brief Weights (multiplicative constants) that linearly stretch individual axes of the feature space.
    * @param weights Values of all weights.
    * @note
    *       WEIGHT_IDX = 0;
    *       X_IDX = 1;
    *       Y_IDX = 2;
    *       L_IDX = 3;
    *       A_IDX = 4;
    *       B_IDX = 5;
    *       CONTRAST_IDX = 6;
    *       ENTROPY_IDX = 7;
    */
    CV_WRAP virtual void setWeights(const std::vector<float>& weights) = 0;

    /**
    * @brief Translations of the individual axes of the feature space.
    * @param idx ID of the translation
    * @param value Value of the translation
    * @note
    *       WEIGHT_IDX = 0;
    *       X_IDX = 1;
    *       Y_IDX = 2;
    *       L_IDX = 3;
    *       A_IDX = 4;
    *       B_IDX = 5;
    *       CONTRAST_IDX = 6;
    *       ENTROPY_IDX = 7;
    */
    CV_WRAP virtual void setTranslation(int idx, float value) = 0;
    /**
    * @brief Translations of the individual axes of the feature space.
    * @param translations Values of all translations.
    * @note
    *       WEIGHT_IDX = 0;
    *       X_IDX = 1;
    *       Y_IDX = 2;
    *       L_IDX = 3;
    *       A_IDX = 4;
    *       B_IDX = 5;
    *       CONTRAST_IDX = 6;
    *       ENTROPY_IDX = 7;
    */
    CV_WRAP virtual void setTranslations(const std::vector<float>& translations) = 0;

    /**
    * @brief Sets sampling points used to sample the input image.
    * @param samplingPoints Vector of sampling points in range [0..1)
    * @note Number of sampling points must be greater or equal to clusterization seed count.
    */
    CV_WRAP virtual void setSamplingPoints(std::vector<Point2f> samplingPoints) = 0;



    /**** clusterizer ****/
    /**
    * @brief Initial seeds (initial number of clusters) for the k-means algorithm.
    */
    CV_WRAP virtual std::vector<int> getInitSeedIndexes() const = 0;
    /**
    * @brief Initial seed indexes for the k-means algorithm.
    */
    CV_WRAP virtual void setInitSeedIndexes(std::vector<int> initSeedIndexes) = 0;
    /**
    * @brief Number of initial seeds (initial number of clusters) for the k-means algorithm.
    */
    CV_WRAP virtual int getInitSeedCount() const = 0;

    /**
    * @brief Number of iterations of the k-means clustering.
    *       We use fixed number of iterations, since the modified clustering is pruning clusters
    *       (not iteratively refining k clusters).
    */
    CV_WRAP virtual int getIterationCount() const = 0;
    /**
    * @brief Number of iterations of the k-means clustering.
    *       We use fixed number of iterations, since the modified clustering is pruning clusters
    *       (not iteratively refining k clusters).
    */
    CV_WRAP virtual void setIterationCount(int iterationCount) = 0;

    /**
    * @brief Maximal number of generated clusters. If the number is exceeded,
    *       the clusters are sorted by their weights and the smallest clusters are cropped.
    */
    CV_WRAP virtual int getMaxClustersCount() const = 0;
    /**
    * @brief Maximal number of generated clusters. If the number is exceeded,
    *       the clusters are sorted by their weights and the smallest clusters are cropped.
    */
    CV_WRAP virtual void setMaxClustersCount(int maxClustersCount) = 0;

    /**
    * @brief This parameter multiplied by the index of iteration gives lower limit for cluster size.
    *       Clusters containing fewer points than specified by the limit have their centroid dismissed
    *       and points are reassigned.
    */
    CV_WRAP virtual int getClusterMinSize() const = 0;
    /**
    * @brief This parameter multiplied by the index of iteration gives lower limit for cluster size.
    *       Clusters containing fewer points than specified by the limit have their centroid dismissed
    *       and points are reassigned.
    */
    CV_WRAP virtual void setClusterMinSize(int clusterMinSize) = 0;

    /**
    * @brief Threshold euclidean distance between two centroids.
    *       If two cluster centers are closer than this distance,
    *       one of the centroid is dismissed and points are reassigned.
    */
    CV_WRAP virtual float getJoiningDistance() const = 0;
    /**
    * @brief Threshold euclidean distance between two centroids.
    *       If two cluster centers are closer than this distance,
    *       one of the centroid is dismissed and points are reassigned.
    */
    CV_WRAP virtual void setJoiningDistance(float joiningDistance) = 0;

    /**
    * @brief Remove centroids in k-means whose weight is lesser or equal to given threshold.
    */
    CV_WRAP virtual float getDropThreshold() const = 0;
    /**
    * @brief Remove centroids in k-means whose weight is lesser or equal to given threshold.
    */
    CV_WRAP virtual void setDropThreshold(float dropThreshold) = 0;

    /**
    * @brief Distance function selector used for measuring distance between two points in k-means.
    */
    CV_WRAP virtual pct_signatures::DistanceFunction getDistanceFunction() const = 0;
    /**
    * @brief Distance function selector used for measuring distance between two points in k-means.
    */
    CV_WRAP virtual void setDistanceFunction(pct_signatures::DistanceFunction distanceFunction) = 0;

};

/**
* @brief Class implementing Signature Quadratic Form Distance (SQFD).
* @see Christian Beecks, Merih Seran Uysal, Thomas Seidl.
*   Signature quadratic form distance.
*   In Proceedings of the ACM International Conference on Image and Video Retrieval, pages 438-445.
*   ACM, 2010.
* @cite BeecksUS10
*/
class CV_EXPORTS_W PCTSignaturesSQFD : public Algorithm
{
public:

    /**
    * @brief Creates the algorithm instance using selected distance function,
    *       similarity function and similarity function parameter.
    * @param distanceFunction Distance function selector.
    * @param similarityFunction Similarity function selector.
    * @param similarityParameter Parameter of the similarity function.
    */
    CV_WRAP static Ptr<PCTSignaturesSQFD> create(
        const pct_signatures::DistanceFunction distanceFunction = pct_signatures::DistanceFunction::L2,
        const pct_signatures::SimilarityFunction similarityFunction = pct_signatures::SimilarityFunction::HEURISTIC,
        const float similarityParameter = 1);

    /**
    * @brief Computes Signature Quadratic Form Distance of two signatures.
    * @param _signature0 The first signature.
    * @param _signature1 The second signature.
    */
    CV_WRAP virtual float computeQuadraticFormDistance(
        const InputArray _signature0,
        const InputArray _signature1) const = 0;

    /**
    * @brief Computes Signature Quadratic Form Distance between the reference signature
    *       and each of the other image signatures.
    * @param sourceSignature The signature to measure distance of other signatures from.
    * @param imageSignatures Vector of signatures to measure distance from the source signature.
    * @param distances Output vector of measured distances.
    */
    CV_WRAP virtual void computeQuadraticFormDistances(
        const Mat& sourceSignature,
        const std::vector<Mat>& imageSignatures,
        std::vector<float>& distances) const = 0;

};

//! @}

}
}

#endif
