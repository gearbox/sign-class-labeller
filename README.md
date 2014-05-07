Tool for labelling traffic sign classes.

For labelling open directory with images. There must be file `in_labelling.txt`
with lines in format

```
sign_id n img1_filename ... imgn_filename
```

While labelling there will be automatically created file `out_labelling.txt`
with labelled classes:


```
sign_id sign_class n img1_filename ... imgn_filename
```

`sign_class` can be `unknown`, `NONE` or image name from `sign-icons/`
directory.
