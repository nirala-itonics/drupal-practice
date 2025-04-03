<?php

function nirala_itonics_products_category_list_page()
{
    $output = '';

    $search_term = isset($_GET['search']) ? $_GET['search'] : '';
    $output .= nirala_itonics_products_category_search_form($search_term);

    $limit = 10;

    $query = db_select('nirala_itonics_products_category', 'p')
        ->fields('p', array('id', 'title', 'status'));

    if (!empty($search_term)) {
        $or_conditions = db_or()
            ->condition('title', '%' . db_like($search_term) . '%', 'LIKE');
        $query->condition($or_conditions);
    }

    $total_products_category = $query->countQuery()->execute()->fetchField();

    if ($total_products_category == 0) {
        $output .= '<p>' . t('No products Category found.') . '</p>';
        return $output;
    }

    $current_page = pager_find_page();
    $offset = $current_page * $limit;

    $query->range($offset, $limit);

    $result = $query->execute();

    $output .= nirala_itonics_products_category_table($result, $offset);

    $output .= nirala_itonics_products_category_pager($total_products_category, $limit);

    return $output;
}


function nirala_itonics_products_category_search_form($search_term = '')
{
    $input_style = 'padding: 10px; border: 1px solid #ccc; border-radius: 5px; font-size: 14px; width: 300px;';
    $button_style = 'padding: 10px 20px; background-color: #007bff; color: #fff; border: none; border-radius: 5px; font-size: 16px;';
    $right_button_style = 'margin-left: auto; padding: 10px 20px; background-color: #007bff; color: #fff; border: none; border-radius: 5px; font-size: 16px;';
    $reset_button_style = 'padding: 10px 20px; background-color: #6c757d; color: #fff; border: none; border-radius: 5px; font-size: 16px;';

    $output = '<div style="display: flex; justify-content: space-between; align-items: center; margin-bottom: 20px;">
               <form method="get" action="" style="display: flex; gap: 10px;">
                   <input type="text" name="search" placeholder="' . t('Search by any field') . '" value="' . htmlspecialchars($search_term) . '" 
                          style="' . $input_style . '"/>
                   <button type="submit" style="' . $button_style . '">' . t('Search') . '</button>
                   <a href="' . url('admin/productscategory/list') . '" style="' . $reset_button_style . '">' . t('Reset') . '</a>
               </form>
               <a href="' . url('admin/productscategory/create') . '" 
                  style="' . $right_button_style . '">' . t('Create Product Category') . '</a>
           </div>';

    return $output;
}


function nirala_itonics_products_category_table($result, $offset)
{
    $output = '<table style="width: 100%; border-collapse: collapse;">';
    $output .= '<tr style="background-color: #f8f9fa;">
                   <th>' . t('S.N.') . '</th>
                   <th>' . t('Title') . '</th>
                   <th>' . t('Status') . '</th>
                   <th>' . t('Actions') . '</th>
               </tr>';

    $serial_no = $offset + 1;

    foreach ($result as $products_category) {
        $output .= '<tr>';
        $output .= '<td>' . $serial_no++ . '</td>';
        $output .= '<td>' . $products_category->title . '</td>';
        $output .= '<td>' . $products_category->status . '</td>';
        $output .= '<td>
        <a href="' . url('admin/productscategory/edit/' . $products_category->id) . '" 
           style="padding: 5px 10px; background-color: #007bff; color: #fff; text-decoration: none; border-radius: 3px; margin-right: 5px;">' . t('Edit') . '</a>
        <a href="' . url('admin/productscategory/delete/' . $products_category->id) . '" 
           style="padding: 5px 10px; background-color: #dc3545; color: #fff; text-decoration: none; border-radius: 3px;">' . t('Delete') . '</a>
    </td>';

        $output .= '</tr>';
    }

    $output .= '</table>';

    return $output;
}

function nirala_itonics_products_category_pager($total_products_category, $limit)
{
    pager_default_initialize($total_products_category, $limit);
    return theme('pager');
}

/**
 * Page callback for product category form.
 */
function nirala_itonics_products_category_form_page()
{
    $form = drupal_get_form('nirala_itonics_products_category_form');
    return drupal_render($form);
}

/**
 * Form for creating or editing a product.
 */
function nirala_itonics_products_category_form($form, &$form_state)
{

    $product_category_id = arg(3);

    if ($product_category_id) {
        $productCategory = db_select('nirala_itonics_products_category', 'p')
            ->fields('p')
            ->condition('id', $product_category_id)
            ->execute()
            ->fetchObject();
    }

    // Title
    $form['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Title'),
        '#required' => TRUE,
        '#default_value' => isset($productCategory->title) ? check_plain($productCategory->title) : '',
    );


    // Status
    $form['status'] = array(
        '#type' => 'radios',
        '#title' => t('Status'),
        '#required' => TRUE,
        '#options' => array(
            1 => t('Active'),
            0 => t('Inactive'),
        ),
        '#default_value' => isset($productCategory->status) ? $productCategory->status : 1,
    );

    // Submit Button
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save Product'),
    );

    return $form;
}

function nirala_itonics_products_category_form_submit($form, &$form_state)
{
    $product_category_id = arg(3);
    $values = $form_state['values'];

    $data = array(
        'title' => $values['title'],
        'status' => $values['status'],
    );


    if (is_numeric($product_category_id)) {
        nirala_itonics_products_category_actions($data, $product_category_id);
    } else {
        nirala_itonics_products_category_actions($data);
    }
    $form_state['redirect'] = 'admin/productscategory';
}

function nirala_itonics_products_category_actions($data, $product_category_id = NULL)
{
    if ($product_category_id) {
        db_update('nirala_itonics_products_category')
            ->fields($data)
            ->condition('id', $product_category_id)
            ->execute();
        drupal_set_message(t('Product Category updated successfully.'));
    } else {
        db_insert('nirala_itonics_products_category')
            ->fields($data)
            ->execute();
        drupal_set_message(t('Product Category created successfully.'));
    }
}

function nirala_itonics_products_category_delete_page()
{
    $product_category_id = arg(3);

    $productCategory = db_select('nirala_itonics_products_category', 'p')
        ->fields('p')
        ->condition('id', $product_category_id)
        ->execute()
        ->fetchObject();

    if (!$productCategory) {
        return t('Product Category not found.');
    }

    $category_usage_count = db_select('nirala_itonics_products_category_map', 'pcm')
        ->fields('pcm', ['id'])
        ->condition('nirala_itonics_products_category_id', $product_category_id)
        ->execute()
        ->rowCount();

    if ($category_usage_count > 0) {
        $output = '<p>' . t('This category is being used in one or more products and cannot be deleted.') . '</p>';
        $output .= '<a href="' . url('admin/productscategory') . '" class="button">' . t('Back to Categories List') . '</a>';
        return $output;
    }

    if (isset($_POST['confirm_delete'])) {
        db_delete('nirala_itonics_products_category')
            ->condition('id', $product_category_id)
            ->execute();

        drupal_set_message(t('Product Category deleted successfully.'));
        return drupal_goto('admin/productscategory');
    }

    $output = '<h2>' . t('Are you sure you want to delete this product category?') . '</h2>';
    $output .= '<p><strong>' . t('Title') . ':</strong> ' . $productCategory->title . '</p>';
    $output .= '<form method="post">';
    $output .= '<input type="submit" name="confirm_delete" value="' . t('Yes, Delete') . '" class="button">';
    $output .= '<a href="' . url('admin/productscategory') . '" class="button">' . t('Cancel') . '</a>';
    $output .= '</form>';

    return $output;
}
