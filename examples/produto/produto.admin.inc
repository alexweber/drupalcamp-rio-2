<?php
/**
 * @file
 * Páginas administrativas para o módulo Produto.
 */

/**
 * Form callback.
 * Generates the produto type edit form.
 */
function produto_type_form($form, &$form_state, $produto_type, $op = 'edit') {
  // Armazena a entidade no $form_state para que possa ser alterada facilmente.
  $form_state['produto_type'] = $produto_type;

  if ($op == 'clone') {
    $produto_type->label .= ' (cloned)';
    $produto_type->name = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $produto_type->label,
    '#description' => t('O nome "humano" deste tipo de produto.'),
    '#required' => TRUE,
    '#size' => 30,
    '#weight' => 0,
  );

  // Machine-readable type name.
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($produto_type->name) ? $produto_type->name
        : '',
    '#maxlength' => 32,
    '#disabled' => $produto_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'produto_get_types',
      'source' => array('label'),
    ),
    '#description' => t('O nome "de máquina" único do tipo de produto. Deve conter apenas letras em caixa baixa, números e underscores.'),
    '#weight' => 1,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => isset($produto_type->description) ? $produto_type->description
        : '',
    '#description' => t('Uma breve descrição deste tipo de produto.'),
    '#weight' => 6,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Salvar tipo de produto'),
    '#weight' => 40,
  );

  if (!$produto_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Excluir tipo de produto'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('produto_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Callback de validação.
 * Valida entidades "tipo de produto".
 *
 * @see produto_type_form()
 */
function produto_type_form_validate(&$form, &$form_state) {
  // @TODO inserir sua validação customizada aqui.
}

/**
 * Callback de submit.
 * Salva tipos de produto.
 *
 * @see produto_type_form()
 */
function produto_type_form_submit(&$form, &$form_state) {
  $produto_type = entity_ui_form_submit_build_entity($form, $form_state);
  if (produto_type_save($produto_type)) {
    if ($form_state['op'] === 'edit') {
      drupal_set_message(t('Tipo de produto %label salvado.', array('%label' => $produto_type->getTranslation('label'))));
    }
    elseif ($form_state['op'] === 'add') {
      drupal_set_message(t('Tipo de produto %label criado.', array('%label' => $produto_type->getTranslation('label'))));
    }
  }
  else {
    drupal_set_message(t('Ocorreu um erro ao salvar o tipo de produto %label.', array('%label' => $produto_type->getTranslation('label'))), 'error');
  }

  $form_state['redirect'] = 'admin/structure/produtos';
}

/**
 * Callback de submit.
 * Exclui tipos de produto.
 *
 * @see produto_type_form()
 */
function produto_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/produtos/manage/' . $form_state['produto_type']->name . '/delete';
}